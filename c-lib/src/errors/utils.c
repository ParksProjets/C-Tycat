/*

Some useful functions for reporting errors.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/errors.h"

#include <stdio.h>
#include <string.h>


// Location information
char _ctycat_loc[255] = "unknown";

// Extra information about an error
char _ctycat_errinfo[255] = "unknown";


// Container for extra information (except location).
static char extra_strz[255];



// Set the location information
void _ctycat_set_location(const char *filename, int lineno)
{
    if (lineno >= 0)
        sprintf(_ctycat_loc, "%s:%d", filename, lineno);
    else
        strcpy(_ctycat_loc, filename);
}


// Set additional error information
void _ctycat_error_info(const char *text)
{
    strcpy(_ctycat_errinfo, text);
}



// Add DWARF error on top the current error text
const char *_ctycat_add_error_dwarf(int error, const char *basetext)
{
    const char *dwarf_err = dwarf_errmsg(error >> 16);
    if (!(error >> 16) || !dwarf_err)
        return basetext;

    sprintf(extra_strz, "%s (%s)", basetext, dwarf_err);
    return extra_strz;
}


// Add errno error on top of the current error text.
const char *_ctycat_add_error_errno(int error, const char *basetext)
{
    char *str_err = strerror(error >> 16);
    sprintf(extra_strz, "%s (%d: %s)", basetext, error >> 16, str_err);
    return extra_strz;
}


// Add error information on top the current error text
const char *_ctycat_add_error_info(const char *basetext)
{
    sprintf(extra_strz, basetext, _ctycat_errinfo);
    return extra_strz;
}


// Add location on top the current error text
const char *_ctycat_add_error_location(const char *basetext)
{
    static char strz[255];
    sprintf(strz, basetext, _ctycat_loc);
    return strz;
}
