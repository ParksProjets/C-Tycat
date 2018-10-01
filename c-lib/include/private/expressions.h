/*

Parse a C expression to tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_EXPRESSIONS_H
#define _CTYCAT_EXPRESSIONS_H

#include <stdint.h>
#include <setjmp.h>

#include <dwarf.h>
#include <elfutils/libdw.h>


// Expression tokens.
enum _Cty_ExpTokens {
    _CTYEXP_WORD  = 1,
    _CTYEXP_CPAR  = 2,
    _CTYEXP_OPAR  = 4,
    _CTYEXP_STAR  = 8,
    _CTYEXP_DOT   = 16,
    _CTYEXP_ARROW = 32,
    _CTYEXP_END   = 64
};


// Additional values parsed by the lexer.
extern char _ctyexp_word[50];

// Cursor position.
extern const char *_ctyexp_lastp;
extern const char *_ctyexp_linep;
extern const char *_ctyexp_line;


// Store useful data when parsing expressions.
struct _ExpressionData {
    void *addr;
    const char *fname;
    Dwarf_Die *die;
};

extern struct _ExpressionData _ctyexp_data;

// Jump buffer for jumping back if an error occurs.
extern jmp_buf _ctyexp_jmp;

// Do we have an error?
extern _Bool _ctyexp_has_error;


// Get the first die.
int _ctycat_getdie(const char *fname, void *addr, const char *vname, Dwarf_Die *die);


// Get the next token from the lexer.
enum _Cty_ExpTokens _ctyexp_next();

// Parse a C expression.
void _ctyexp_parse(const char *expr);

// Stop the parser on an error.
void _ctyexp_error(int error);

// Adavnce the die to the type die.
void _ctyexp_advance_to_type();


// Parse a dereference (*) expression.
void _ctyexp_dereference();

// Parse a dot access (.) expression.
void _ctyexp_dot_access();

// Parse an arrow access (->) expression.
void _ctyexp_arrow_access();


// Find the variable tycated at a line.
void _ctyexp_find_var();


#endif  // _CTYCAT_EXPRESSIONS_H
