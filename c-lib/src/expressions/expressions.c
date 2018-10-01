/*

Parse a C expression to tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/expressions.h"
#include "private/errors.h"

// Store useful data when parsing expressions.
struct _ExpressionData _ctyexp_data;

// Do we have an error?
_Bool _ctyexp_has_error;



// Get the first die.
int _ctycat_getdie(const char *fname, void *addr, const char *vname, Dwarf_Die *die)
{
    _ctyexp_data.addr = addr;
    _ctyexp_data.fname = fname;
    _ctyexp_data.die = die;

    _ctyexp_has_error = 0;
    _ctyexp_parse(vname);

    return _ctyexp_has_error;
}



// Stop the parser on an error.
void _ctyexp_error(int error)
{
    _ctycat_report_error(error);
    _ctyexp_has_error = 1;

    longjmp(_ctyexp_jmp, 1);
}



// Return true if a die is a qualifier.
static inline int isqualifier(Dwarf_Die *d) {
    int type = dwarf_tag(d);
    return
        type == DW_TAG_const_type    ||
        type == DW_TAG_volatile_type ||
        type == DW_TAG_restrict_type ||
        type == DW_TAG_typedef;
}

// Adavnce the die to the type die.
void _ctyexp_advance_to_type()
{
    Dwarf_Attribute attr;
    while (isqualifier(_ctyexp_data.die)) {
        dwarf_attr(_ctyexp_data.die, DW_AT_type, &attr);
        dwarf_formref_die(&attr, _ctyexp_data.die);
    }
}
