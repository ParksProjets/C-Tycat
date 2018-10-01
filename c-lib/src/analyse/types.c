/*

Some useful functons for manipulting types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"
#include "private/ctycat.h"



// Format a Libdw die.
void _ctycat_format_die(Dwarf_Off offset, _Cty_Die *die)
{
    inline int isqualifier(Dwarf_Die *d) {
        int type = dwarf_tag(d);
        return
            type == DW_TAG_const_type    ||
            type == DW_TAG_volatile_type ||
            type == DW_TAG_restrict_type ||
            type == DW_TAG_typedef;
    }


    dwarf_offdie(_ctycat_dwarf, offset, &die->qualifiers);

    Dwarf_Attribute attr;
    die->type = die->qualifiers;

    while (isqualifier(&die->type)) {
        dwarf_attr(&die->type, DW_AT_type, &attr);
        dwarf_formref_die(&attr, &die->type);
    }
}
