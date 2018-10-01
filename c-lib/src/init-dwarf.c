/*

Initialize C-Tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/ctycat.h"
#include "private/errors.h"

#define __USE_BSD 1  // unistd.h doesn't like C99
#include <unistd.h>
#include <fcntl.h>

#include <string.h>


// Dummy
static char *debuginfo_path;

// Libdwfl offline callbacks
static const Dwfl_Callbacks offline_callbacks = {
    .find_debuginfo = dwfl_standard_find_debuginfo,
    .debuginfo_path = &debuginfo_path,
    .section_address = dwfl_offline_section_address,
    .find_elf = dwfl_build_id_find_elf,
};


// Is C-Tycat initialized?
_Bool _ctycat_initialized;


// Dwarf and Dwfl containers
Dwarf *_ctycat_dwarf;
Dwfl *_ctycat_dwfl;

// Hashmap of CU dies
khash_t(_cuhash) *_ctycat_cumap;




// Load dwarf table from executable
static int _ctycat_load_dwarf()
{
    Dwfl_Module *mod = NULL;
    Dwarf_Addr bias;

    char exepath[255] = "unknown";
    readlink("/proc/self/exe", exepath, 254);

    int fd = open(exepath, O_RDONLY);
    if (fd < 0)
        return _CTY_ERR_READ_EXE;

    _ctycat_dwfl = dwfl_begin(&offline_callbacks);
    if (!_ctycat_dwfl)
        goto error;

    dwfl_report_begin(_ctycat_dwfl);
    mod = dwfl_report_offline(_ctycat_dwfl, "", "", fd);
    if (!mod)
        goto error;

    _ctycat_dwarf = dwfl_module_getdwarf(mod, &bias);
    if (!_ctycat_dwarf)
        goto error;

    dwfl_report_end(_ctycat_dwfl, NULL, NULL);

    close(fd);
    return _CTY_NO_ERROR;

error:
    if (_ctycat_dwfl)
        dwfl_end(_ctycat_dwfl);

    close(fd);
    return _CTY_ERR_DWARF(_CTY_ERR_LOAD_DWARF);
}




// Load all CU dies into the hash map.
static int _ctycat_load_cudies()
{
    Dwarf_Off next, off = 0;
    Dwarf_Die die;
    size_t sz;

    int dummy;
    _ctycat_cumap = kh_init(_cuhash);

    while (!dwarf_nextcu(_ctycat_dwarf, off, &next, &sz, NULL, NULL, NULL)) {
        if (dwarf_offdie(_ctycat_dwarf, off + sz, &die) == NULL)
            return _CTY_ERR_DWARF(_CTY_ERR_LOAD_CUDIES);

        khiter_t k = kh_put(_cuhash, _ctycat_cumap, dwarf_diename(&die), &dummy);
        kh_value(_ctycat_cumap, k) = off + sz;

        off = next;
    }

    return _CTY_NO_ERROR;
}




// Initialize CTycat
int _ctycat_init()
{
    int ret;

    if (_ctycat_initialized) return _CTY_NO_ERROR;
    if (_ctycat_error_reported) return _CTY_FATAL_ERROR;

    if (ret = _ctycat_load_dwarf())
        return _ctycat_report_error(ret);

    if (ret = _ctycat_load_cudies())
        return _ctycat_report_error(ret);

    _ctycat_initialized = 1;
    return _CTY_NO_ERROR;
}
