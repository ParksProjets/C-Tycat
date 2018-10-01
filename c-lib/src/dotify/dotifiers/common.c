/*

Dotify common types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Write a base type.
void _ctydot_basetype(_Cty_Die *die, void *data)
{
    node_label(data);
    fputs("[label=<", f);

    _ctydot_value(die, data);
    fputs(">];\n", f);
}



// Dotify a type at the given address.
void _ctydot_dotify(Dwarf_Off offset, void *data)
{
    _Cty_Die die;
    _ctycat_format_die(offset, &die);

    // TODO: is array?

    int type = dwarf_tag(&die.type);

    if (type == DW_TAG_structure_type)
        _ctydot_struct(&die, data);

    else if (type == DW_TAG_pointer_type)
        _ctydot_pointer(&die, data);

    // TODO.
    else if (type == DW_TAG_array_type)
        return;

    else
        _ctydot_basetype(&die, data);
}



// Dotify a type at the given address.
void _ctydot_dotify_inline(Dwarf_Off offset, void *data)
{
    _Cty_Die die;
    _ctycat_format_die(offset, &die);

    int type = dwarf_tag(&die.type);

    if (type == DW_TAG_structure_type)
        _ctydot_struct_inline(&die, data);

    else if (type == DW_TAG_pointer_type)
        _ctydot_pointer_inline(&die, data);

    // TODO.
    else if (type == DW_TAG_array_type)
        return;

    else
        _ctydot_value(&die, data);
}
