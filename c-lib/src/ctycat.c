/*

Load all debug symbols from the executable.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/ctycat.h"
#include "private/expressions.h"
#include "private/errors.h"

#include <stdio.h>


// Number of the current C-Tycat (starts at one).
int _ctycat_number;



// Raw function for C tycat
void _ctycat(const char *fname, int line, void *addr, const char *vname, void *data)
{
    if (_ctycat_init_dward())
        return; // An error has occured.

    Dwarf_Die typedie;
    _ctycat_set_location(fname, line);

    if (_ctycat_getdie(fname, addr, vname, &typedie))
        return; // An error has occured.

    _ctycat_number++;
    printf("Tycat variable '%s' in %s:%d\n", vname, fname, line);

    _ctycat_dotify(&typedie, data, vname);
}



// Uninitialize C-Tycat.
void _ctycat_uninit()
{
    if (!_ctycat_initialized)
        return;

    dwfl_end(_ctycat_dwfl);
    kh_destroy(_cuhash, _ctycat_cumap);
}
