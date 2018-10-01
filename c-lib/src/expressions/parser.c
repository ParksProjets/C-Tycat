/*

Parse a C expression to tycat with an LL(1) like parser.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/expressions.h"
#include "private/errors.h"


// Next token look-ahead
static enum _Cty_ExpTokens lookup;

// Jump buffer for jumping back if an error occurs.
jmp_buf _ctyexp_jmp;


// Prototype of the 'parse' functions.
static void parse_cast();
static void parse_exp1();
static void parse_exp0bis();
static void parse_exp0();



// Expect a token to be parsed and update the look-ahead.
static void expect(int tokens)
{
    if ((lookup & tokens) == 0)
        _ctyexp_error(_CTY_ERR_EXP);

    lookup = _ctyexp_next();
}




// Parse a type cast.
static void parse_cast()
{
    expect(_CTYEXP_OPAR);

    // TODO.

    expect(_CTYEXP_CPAR);    
}




// Parse an expression with priority of 1.
static void parse_exp1()
{
    // Parse an expression with priority of 0.
    if (lookup == _CTYEXP_WORD)
        return parse_exp0();

    // Parse a dereference.
    if (lookup == _CTYEXP_STAR) {
        expect(_CTYEXP_STAR);
        parse_exp1();
        return _ctyexp_dereference();
    }

    // The token must be an open parenthesis.
    if (lookup != _CTYEXP_OPAR)
        expect(_CTYEXP_OPAR);

    // if (!_ctyexp_is_cast())
    //     return parse_exp0();

    // Parse a type cast.
    // parse_cast();
    // _ctyexp_type_cast();


    // FIXME: remove this line.
    parse_exp0();
}




// Parse an expression with priority of 0.
static void parse_exp0bis()
{
    // Parse an arrow '->' access.
    if (lookup == _CTYEXP_ARROW) {
        expect(_CTYEXP_ARROW);
        expect(_CTYEXP_WORD);
        _ctyexp_arrow_access();
        return parse_exp0bis();
    }

    // Parse a dot '.' access.
    if (lookup == _CTYEXP_DOT) {
        expect(_CTYEXP_DOT);
        expect(_CTYEXP_WORD);
        _ctyexp_dot_access();
        return parse_exp0bis();
    }

    // Parse epsilon.
    if (lookup != _CTYEXP_CPAR && lookup != _CTYEXP_END)
        expect(_CTYEXP_OPAR | _CTYEXP_END);
}



// Parse an expression with priority of 0.
static void parse_exp0()
{
    _Bool is_word = (lookup == _CTYEXP_WORD);
    expect(_CTYEXP_WORD | _CTYEXP_OPAR);

    if (!is_word) {
        parse_exp1();
        expect(_CTYEXP_CPAR);
    }
    else {
        _ctyexp_find_var();
    }

    parse_exp0bis();
}




// Parse a C expression.
void _ctyexp_parse(const char *expr)
{
    _ctyexp_line = _ctyexp_linep = expr;
    lookup = _ctyexp_next();

    if (setjmp(_ctyexp_jmp) == 0)
        parse_exp0();
}
