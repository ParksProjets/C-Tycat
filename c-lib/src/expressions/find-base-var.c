/*

Find the base variable definition of the C expression.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/expressions.h"
#include "private/ctycat.h"
#include "private/errors.h"

#include <stdlib.h>



// Get the CU die associated with a filename
static int get_cudie(const char *filename, Dwarf_Die *die)
{
    khiter_t k = kh_get(_cuhash, _ctycat_cumap, filename);
    if (k == kh_end(_ctycat_cumap))
        return _CTY_ERR_FIND_CU;

    Dwarf_Off off = kh_value(_ctycat_cumap, k);

    if (dwarf_offdie(_ctycat_dwarf, off, die) == NULL)
        return _CTY_ERR_DWARF(_CTY_ERR_LOAD_CUDIES);

    return _CTY_NO_ERROR;
}




// Find the definition of the variable
static int find_vardef(Dwarf_Die *cudie, void *addr, const char *vname,
    Dwarf_Die *vardie)
{
    Dwarf_Die *scopes = NULL;

    int len = dwarf_getscopes(cudie, (Dwarf_Addr)addr, &scopes);
    if (len <= 0)
        return _CTY_ERR_DWARF(_CTY_ERR_FIND_SCOPES);

    int ret = dwarf_getscopevar(scopes, len, vname, 0, NULL, 0, 0, vardie);
    free(scopes);

    if (ret == -1)
        return _CTY_ERR_DWARF(_CTY_ERR_FIND_VARDEF);
    else if (ret == -2)
        return _CTY_ERR_FIND_VARDEF;

    return _CTY_NO_ERROR;
}




// Get the type of the variable
static int find_vartype(Dwarf_Die *vardie, Dwarf_Die *typedie)
{
    Dwarf_Attribute attr;

    if (!dwarf_attr(vardie, DW_AT_type, &attr))
        return _CTY_ERR_FIND_VARTYPE;

    dwarf_formref_die(&attr, typedie);
    return _CTY_NO_ERROR;
}




// Find the variable tycated at a line.
void _ctyexp_find_var()
{
    int ret;
    Dwarf_Die cudie, vardie;

    if (ret = get_cudie(_ctyexp_data.fname, &cudie))
        return _ctyexp_error(ret);

    if (ret = find_vardef(&cudie, _ctyexp_data.addr, _ctyexp_word, &vardie))
        return _ctyexp_error(ret);

    if (ret = find_vartype(&vardie, _ctyexp_data.die))
        return _ctyexp_error(ret);
}
