/*

Report C-Tycat warnings to the user.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/errors.h"

#include <stdio.h>



// Get a text coresponding to a warning
const char *_ctycat_warn_text(int warning)
{
    switch (warning & 0xFFFF) {
        case _CTY_WARN_CONF_KEY:
            return _info("Option '%s' doesn't exist");
        case _CTY_WARN_CONF_TYPE:
            return _info("You don't use the right type for option '%s'.");

        default:
            return "Unknown warning";
    }
}



// Warning colors
#define _CTYCOL_RESET "\033[0m"
#define _CTYCOL_BOLD "\033[1m"
#define _CTYCOL_BOLDMAGENTA _CTYCOL_BOLD "\033[35m\033[95m"
#define _CTYCOL_BOLDYELLOW _CTYCOL_BOLD "\033[33;1m"
#define _CTYCOL_YELLOW _CTYCOL_RESET "\033[33m"

// Print and format a warning
#define _warnf(msg, ...) fprintf(stderr, _CTYCOL_BOLDMAGENTA "!! " \
    _CTYCOL_RESET msg _CTYCOL_RESET "\n", ## __VA_ARGS__)


// Report an warning
int _ctycat_report_warning(int warning)
{
    const char *strz = _ctycat_warn_text(warning);

    fprintf(stderr, "\n");
    _warnf(_CTYCOL_BOLDYELLOW "ctycat: a warning has occurred");
    _warnf(_CTYCOL_YELLOW "  [" _CTYCOL_BOLDYELLOW "%d" _CTYCOL_YELLOW
        "] %s\n", warning & 0xFFFF, strz);

    return warning;
}
