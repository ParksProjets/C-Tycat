/*

Analyse a structure.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"



// Analyse a struct member.
static void _ctyan_struct_field(Dwarf_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word offset;

    dwarf_attr(die, DW_AT_data_member_location, &attr);
    dwarf_formudata(&attr, &offset);

    // TODO: d->struct_addr
    // unsigned int shift = (int)((char *)data - (char *)d->struct_addr + offset);
    unsigned int shift = 0; 

    Dwarf_Die ndie;
    dwarf_attr(die, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &ndie);

    _ctyan_analyse_inline(dwarf_dieoffset(&ndie), ((char *)data) + offset);
}



// Analyse the content of a structure.
void _ctyan_struct(_Cty_Die *die, void *data)
{
    Dwarf_Die child;
    if (dwarf_child(&die->type, &child))
        return;

    do {
        _ctyan_struct_field(&child, data);
    } while (dwarf_siblingof(&child, &child) == 0);
}
