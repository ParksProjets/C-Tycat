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
    static char strz[255];

    const char *dwarf_err = dwarf_errmsg(error >> 16);
    if (!(error >> 16) || !dwarf_err)
        return basetext;

    sprintf(strz, "%s (%s)", basetext, dwarf_err);
    return strz;
}



// Add location on top the current error text
const char *_ctycat_add_error_location(const char *basetext)
{
    static char strz[255];
    sprintf(strz, basetext, _ctycat_loc);
    return strz;
}



// Add error information on top the current error text
const char *_ctycat_add_error_info(const char *basetext)
{
    static char strz[255];
    sprintf(strz, basetext, _ctycat_errinfo);
    return strz;
}
