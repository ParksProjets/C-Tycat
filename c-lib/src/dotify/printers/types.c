/*

Some useful functons for printing types. 

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"
#include "private/ctycat.h"



// Return the offset of the typedef (or 0 if no typedef).
static Dwarf_Off typedef_offset(_Cty_Die *fdie)
{
    Dwarf_Attribute attr;
    Dwarf_Off current;

    Dwarf_Die die = fdie->qualifiers;
    Dwarf_Off end = dwarf_dieoffset(&fdie->type);

    while ((current = dwarf_dieoffset(&die)) != end) {
        if (dwarf_tag(&die) == DW_TAG_typedef)
            return current;

        dwarf_attr(&die, DW_AT_type, &attr);
        dwarf_formref_die(&attr, &die);
    }

    return 0;
}



// Print type qualifiers (and return 1 if there are qualifiers).
static _Bool print_qualifiers(_Cty_Die *fdie, Dwarf_Off end)
{
    Dwarf_Die die = fdie->qualifiers;
    Dwarf_Attribute attr;

    Dwarf_Off start = dwarf_dieoffset(&die);
    Dwarf_Off current;

    while ((current = dwarf_dieoffset(&die)) != end) {
        switch (dwarf_tag(&die)) {
            case DW_TAG_const_type:
                fputs("const", f); break;
            case DW_TAG_volatile_type:
                fputs("volatile", f); break;
            case DW_TAG_restrict_type:
                fputs("restrict", f); break;
        }

        dwarf_attr(&die, DW_AT_type, &attr);
        dwarf_formref_die(&attr, &die);

        fputs(" ", f);
    }

    return start != end;
}




// Print the node name from a pointer.
static void print_ptr(_Cty_Die *fdie)
{
    Dwarf_Die die;
    Dwarf_Attribute attr;

    dwarf_attr(&fdie->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &die);

    _Cty_Die pointed;
    _ctycat_format_die(dwarf_dieoffset(&die), &pointed);

    _ctydot_typename(&pointed);
    fputs(" *", f);

    if (print_qualifiers(fdie, dwarf_dieoffset(&fdie->type)))
        fseek(f, -1, SEEK_CUR);
}



// Print the type of a node.
void _ctydot_typename(_Cty_Die *die)
{
    int type = dwarf_tag(&die->type);
    Dwarf_Off offset = typedef_offset(die);

    if (!offset && type == DW_TAG_pointer_type)
        return print_ptr(die);

    if (!offset)
        offset = dwarf_dieoffset(&die->type);

    print_qualifiers(die, offset);

    Dwarf_Die rdie;
    dwarf_offdie(_ctycat_dwarf, offset, &rdie);

    if (dwarf_diename(&rdie))
        fputs(dwarf_diename(&rdie), f);
}
