/*

Parse a C expression to tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/expressions.h"
#include "private/errors.h"

#include <string.h>
#include <ctype.h>


// Additional values parsed by the lexer.
char _ctyexp_word[50];

// Cursor position.
const char *_ctyexp_lastp;
const char *_ctyexp_linep;
const char *_ctyexp_line;

// Shortname macros
#define pos _ctyexp_linep
#define last _ctyexp_lastp
#define n _ctyexp_interger



// Is the expression in parenthesis a type cast?
static _Bool _ctyexp_is_cast()
{
    char *curs = pos;
    while (*curs != ')' && *curs != '\0')
        curs++;

    if (*curs != '\0') return 0;
    curs++;

    while (isspace(*curs))
        curs++;

    return isalpha(*curs) || *curs == '_' || *curs == '*' || *curs == '(';
}



// Parse a word.
static void parse_word()
{
    char *start = pos - 1;
    while (isalnum(*pos) || *pos == '_')
        pos++;

    if (pos - start >= 50)
        _ctyexp_error(_CTY_ERR_EXP);

    strncpy(_ctyexp_word, start, pos - start);
    _ctyexp_word[pos - start] = '\0';
}



// Get the next token from the lexer.
enum _Cty_ExpTokens _ctyexp_next()
{
    while (isspace(*pos))
        pos++;

    last = pos;
    pos++;

    switch (*last) {
        case '\0':
            pos--;
            return _CTYEXP_END;

        case ')':
            return _CTYEXP_CPAR;
        case '(':
            return _CTYEXP_OPAR;
        case '*':
            return _CTYEXP_STAR;

        case '.':
            return _CTYEXP_DOT;
        case '-':
            if (*(pos++) == '>')
                return _CTYEXP_ARROW;
            _ctyexp_error(_CTY_ERR_EXP);

        default:
            if (isalpha(*last) || *last == '_')
                return (parse_word(), _CTYEXP_WORD);
            _ctyexp_error(_CTY_ERR_EXP);
    }
}
