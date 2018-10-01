/*

Manage access (., ->, *) expressions.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/expressions.h"
#include "private/errors.h"

#include <string.h>



// Parse a dereference (*) expression.
void _ctyexp_dereference()
{
    _ctyexp_advance_to_type();

    // Ensure the die is a pointer.
    int type = dwarf_tag(_ctyexp_data.die);
    if (type != DW_TAG_pointer_type)
        _ctyexp_error(_CTY_ERR_EXP);

    // Dereference the die.
    Dwarf_Attribute attr;
    dwarf_attr(_ctyexp_data.die, DW_AT_type, &attr);
    dwarf_formref_die(&attr, _ctyexp_data.die);
}



// Parse a dot access (.) expression.
void _ctyexp_dot_access()
{
    _ctyexp_advance_to_type();

    // Ensure the die is a structure.
    int type = dwarf_tag(_ctyexp_data.die);
    if (type != DW_TAG_structure_type)
        _ctyexp_error(_CTY_ERR_EXP);

    // Ensure the die has at least one child.
    Dwarf_Die child;
    if (dwarf_child(_ctyexp_data.die, &child))
        _ctyexp_error(_CTY_ERR_EXP);


    // Find the right child.
    do {
        if (strcmp(dwarf_diename(&child), _ctyexp_word) != 0)
            continue;

        Dwarf_Attribute attr;
        dwarf_attr(&child, DW_AT_type, &attr);
        return (void)dwarf_formref_die(&attr, _ctyexp_data.die);
    } while (dwarf_siblingof(&child, &child) == 0);

    _ctyexp_error(_CTY_ERR_EXP);
}



// Parse an arrow access (->) expression.
void _ctyexp_arrow_access()
{
    _ctyexp_dereference();
    _ctyexp_dot_access();
}
