/*

Parse C-Tycat configuration files.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/config.h"
#include "private/errors.h"

#include <string.h>
#include <ctype.h>


// Additional values parsed by the lexer.
int _ctyconf_interger;
char _ctyconf_string[50];
char _ctyconf_word[40];

// Cursor position.
char *_ctyconf_lastp;
char *_ctyconf_linep;
char *_ctyconf_line;


// Shortname macros
#define pos _ctyconf_linep
#define last _ctyconf_lastp
#define n _ctyconf_interger



// Parse a string.
static void parse_string()
{
    char *start = pos;
    while (*pos != '"' && *pos != '\0')
        pos++;

    if (*pos != '"')
        _ctyconf_error("Unexpected end of string");
    if (pos - start >= 50)
        _ctyconf_error("String to long (more than 50 chars)");

    strncpy(_ctyconf_string, start, pos - start);
    _ctyconf_string[pos - start] = '\0';

    pos++;
}



// Parse an integer.
static void parse_interger()
{
    n = *(pos - 1) - '0';

    for (; isdigit(*pos); pos++)
        n = (n * 10) + (*pos - '0');
}



// Parse a word.
static enum _Cty_ConfTokens parse_word()
{
    char *start = pos - 1;
    while (isalnum(*pos) || *pos == '-' || *pos == '_')
        pos++;

    if (strncmp(start, "true", pos - start) == 0) {
        _ctyconf_interger = 1;
        return _CTYTOK_BOOLEAN;
    }
    if (strncmp(start, "false", pos - start) == 0) {
        _ctyconf_interger = 0;
        return _CTYTOK_BOOLEAN;
    }

    if (pos - start >= 40)
        _ctyconf_error("Word to long (more than 40 chars)");

    strncpy(_ctyconf_word, start, pos - start);
    _ctyconf_word[pos - start] = '\0';

    return _CTYTOK_WORD;
}



// Get the next token from the lexer.
enum _Cty_ConfTokens _ctyconf_next()
{
    while (isspace(*pos))
        pos++;

    last = pos;
    pos++;

    if (*last == '\0' || *last == '#')
        return (pos--, _CTYTOK_END);
    if (*last == '=')
        return _CTYTOK_ASSIGN;
    if (*last == '"')
        return (parse_string(), _CTYTOK_STRING);
    if (isdigit(*last))
        return (parse_interger(), _CTYTOK_INTEGER);
    if (isalpha(*last))
        return parse_word();

    _ctyconf_error("Unexpected character '%c'", *last);
}
