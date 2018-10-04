/*

Report C-Tycat errors to the user.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/errors.h"

#include <stdio.h>


// Have we already reported an error?
int _ctycat_error_reported;



// Get the text corresponding to an error.
const char *_ctycat_error_text(int error)
{
    switch (error & 0xFFFF) {
        case _CTY_NO_ERROR:
            return "No error";
        case _CTY_FATAL_ERROR:
            return "Fatal error";

        case _CTY_ERR_READ_EXE:
            return "Can't read the current executable";
        case _CTY_ERR_LOAD_DWARF:
            return _dwarf("Can't load DWARF data, ensure you used -g");

        case _CTY_ERR_LOAD_CUDIES:
            return _dwarf("Can't get a CU die");
        case _CTY_ERR_OPEN_DOT:
            return _errno("Can't open DOT file");

        case _CTY_ERR_FIND_CU:
            return _loc("Can't find CU for %s, ensure you used -g");
        case _CTY_ERR_FIND_SCOPES:
            return _dwarf(_loc("Can't find scopes containing %s"));
        case _CTY_ERR_FIND_VARDEF:
            return _dwarf(_loc("Can't find variable definition for %s"));
        case _CTY_ERR_FIND_VARTYPE:
            return _loc("Can't find variable type for %s");

        case _CTY_ERR_EXP:
            return _loc("Error while parsing expression at %s");

        case _CTY_ERR_CONF_READ:
            return _loc("Can't read config file '%s'");
        case _CTY_ERR_CONF_PARSE:
            return _loc("Parsing error in config file '%s'");

        default:
            return "Unknown error";
    }
}



// Terminal colors.
#define TTY_RESET "\033[0m"
#define TTY_BOLD "\033[1m"
#define TTY_RED "\033[31m"
#define TTY_MAGENTA "\033[35m"

// Print and format an error.
#define _ctycat_errf(msg, ...) fprintf(stderr, TTY_BOLD TTY_MAGENTA "!! " \
    TTY_RESET msg TTY_RESET "\n", ## __VA_ARGS__)


// Report an error
int _ctycat_report_error(int error)
{
    if (error == _CTY_NO_ERROR)
        return _CTY_NO_ERROR;

    _ctycat_error_reported |= (error < 500);

    const char *strz = _ctycat_error_text(error);

    fprintf(stderr, "\n");
    _ctycat_errf(TTY_BOLD TTY_RED "ctycat: an error has occurred" TTY_RESET);
    _ctycat_errf(TTY_RED "  [" TTY_BOLD "%d" TTY_RESET TTY_RED "] %s\n",
        error & 0xFFFF, strz);

    return _CTY_FATAL_ERROR;
}
