/*

Manage C-Tycat errors.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_ERRORS_H
#define _CTYCAT_ERRORS_H

#include <elfutils/libdw.h>
#include <errno.h>


// All error types.
enum _Ctycat_Errors {
    _CTY_NO_ERROR = 0,
    _CTY_FATAL_ERROR = 1,

    _CTY_ERR_READ_EXE = 101,
    _CTY_ERR_LOAD_DWARF = 102,

    _CTY_ERR_LOAD_CUDIES = 201,
    _CTY_ERR_OPEN_DOT = 202,

    _CTY_ERR_FIND_CU = 301,
    _CTY_ERR_FIND_SCOPES = 302,
    _CTY_ERR_FIND_VARDEF = 303,
    _CTY_ERR_FIND_VARTYPE = 304,

    _CTY_ERR_EXP = 401,

    _CTY_ERR_CONF_READ = 701,
    _CTY_ERR_CONF_PARSE = 702
};


// All warning types.
enum _Ctycat_Warnings {
    _CTY_WARN_CONF_KEY = 751,
    _CTY_WARN_CONF_TYPE = 752
};


// Add Libdw error on top of C-Tycat error.
#define _CTY_ERR_DWARF(err) (err | (dwarf_errno() << 16))

// Add errno on top of C-Tycat error.
#define _CTY_ERR_ERRNO(err) (err | (errno << 16))



// Location information.
extern char _ctycat_loc[255];

// Extra information about an error.
extern char _ctycat_errinfo[255];

// Set the location information.
void _ctycat_set_location(const char *filename, int lineno);

// Set additional error information.
void _ctycat_error_info(const char *text);



// Shortname macros
#define _dwarf(msg) _ctycat_add_error_dwarf(error, msg)
#define _errno(msg) _ctycat_add_error_errno(error, msg)
#define _info(msg) _ctycat_add_error_info(msg)
#define _loc(msg) _ctycat_add_error_location(msg)


// Add DWARF error on top of the current error text.
const char *_ctycat_add_error_dwarf(int error, const char *basetext);

// Add errno error on top of the current error text.
const char *_ctycat_add_error_errno(int error, const char *basetext);

// Add location on top the current error text.
const char *_ctycat_add_error_location(const char *basetext);

// Add error information on top the current error text.
const char *_ctycat_add_error_info(const char *basetext);



// Get a text coresponding to an error.
const char *_ctycat_error_text(int error);

// Report an error.
int _ctycat_report_error(int error);



// Get a text coresponding to a warning.
const char *_ctycat_warn_text(int warning);

// Report a warning.
int _ctycat_report_warning(int warning);


#endif // _CTYCAT_ERRORS_H
