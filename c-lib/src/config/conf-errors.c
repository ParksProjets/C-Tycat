/*

Parse C-Tycat configuration files.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/config.h"
#include "private/errors.h"

#include <stdio.h>
#include <stdarg.h>
#include <strings.h>


// Is the parsing error reported?
_Bool _ctyconf_error_reported;

// Terminal colors.
#define TTY_RESET "\033[0m"
#define TTY_BOLD "\033[1m"
#define TTY_MAGENTA "\033[35m"



// Print the error position.
static void print_pos()
{
    fprintf(stderr, TTY_MAGENTA "\n    %s\n    ", _ctyconf_line);

    char *p = _ctyconf_line;
    for (; p < _ctyconf_lastp; p++)
        fprintf(stderr, " ");

    fprintf(stderr, "^");

    for (p++; p < _ctyconf_linep; p++)
        fprintf(stderr, "~");
}



// Print a parser error.
void _ctyconf_error(const char *msg, ...)
{
    if (!_ctyconf_error_reported) {
        _ctyconf_error_reported = 1;
        _ctycat_report_error(_CTY_ERR_CONF_PARSE);
    }

    va_list argp;
    va_start(argp, msg);

    fprintf(stderr, TTY_BOLD "  line %d: " TTY_RESET, _ctyconf_lineno);
    vfprintf(stderr, msg, argp);

    if (_ctyconf_lastp < _ctyconf_linep)
        print_pos();

    fprintf(stderr, TTY_RESET "\n\n");
    va_end(argp);

    longjmp(_ctyconf_jmp, 1);
}



// Print a token error.
void _ctyconf_error_token(enum _Cty_ConfTokens token)
{
    static const char *names[] = {
        "word (maybe put it in quotes?)", "integer", "string", "boolean",
        "assignation", "end of line"
    };

    _ctyconf_error("unexpected %s", names[ffs(token) - 1]);
}
