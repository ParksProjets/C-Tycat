/*

Parse C-Tycat configuration files.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/config.h"
#include "private/errors.h"

#include <stdio.h>


// Next token look-ahead
static enum _Cty_ConfTokens lookup;

// Current line number
int _ctyconf_lineno;

// Jump buffer for jumping back if an error occurs.
jmp_buf _ctyconf_jmp;



// Set the current parameter.
static void set_parameter(enum _Cty_ConfTokens type)
{
    khiter_t k = kh_get(_conf, _ctycat_confmap, _ctyconf_word);

    if (k == kh_end(_ctycat_confmap))
        _ctyconf_error("parameter '%s' doesn't exist", _ctyconf_word);

    int ctype = (type == _CTYTOK_STRING);
    if (kh_value(_ctycat_confmap, k).type != ctype)
        _ctyconf_error("wrong type for parameter '%s'", _ctyconf_word);

    if (ctype == _CTYCONF_INTEGER) {
        *(int *)kh_value(_ctycat_confmap, k).value = _ctyconf_interger;
    } else {
        free(*(char **)kh_value(_ctycat_confmap, k).value);
        *(char **)kh_value(_ctycat_confmap, k).value = strdup(_ctyconf_string);
    }
}




// Expect a token to be parsed and update the look-ahead.
static void expect(int tokens)
{
    if ((lookup & tokens) == 0)
        _ctyconf_error_token(lookup);

    lookup = _ctyconf_next();
}



// Parse a line (an assignation)
static void parse_line()
{
    lookup = _ctyconf_next();
    if (lookup == _CTYTOK_END) return;

    expect(_CTYTOK_WORD);
    expect(_CTYTOK_ASSIGN);

    enum _Cty_ConfTokens type = lookup;
    expect(_CTYTOK_INTEGER | _CTYTOK_STRING | _CTYTOK_BOOLEAN);
    expect(_CTYTOK_END);

    set_parameter(type);
}



// Read a configuration file.
void ctycat_read_config(const char *filename)
{
    FILE * fp = fopen(filename, "r");
    _ctycat_set_location(filename, -1);

    if (!fp)
        return (void)_ctycat_report_error(_CTY_ERR_CONF_READ);

    char buffer[512];
    _ctyconf_lineno = 0;
    _ctyconf_error_reported = 0;

    while (fgets(buffer, 512, fp) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        _ctyconf_lineno++;
        _ctyconf_line = _ctyconf_linep = buffer;

        if (setjmp(_ctyconf_jmp) == 0)
            parse_line();
    }

    fclose(fp);
}
