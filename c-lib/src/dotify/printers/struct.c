/*

Print structure specific objects.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Print a typedef.
static void print_typedef(Dwarf_Die *die)
{
    fprintf(f, "%s", dwarf_diename(die));
}



// Print structure typedef (return '0' if no typedef).
static _Bool find_and_print_typedef(_Cty_Die *fdie)
{
    Dwarf_Attribute attr;
    Dwarf_Off current;

    Dwarf_Die die = fdie->qualifiers;
    Dwarf_Off end = dwarf_dieoffset(&fdie->type);

    while ((current = dwarf_dieoffset(&die)) != end) {
        if (dwarf_tag(&die) == DW_TAG_typedef)
            return (print_typedef(&die), 1);

        dwarf_attr(&die, DW_AT_type, &attr);
        dwarf_formref_die(&attr, &die);
    }

    return 0;
}



// Print the name of a structure.
void _ctydot_struct_name(_Cty_Die *die)
{
    fputs("  <TR><TD COLSPAN=\"2\" bgcolor=\"gray82\" port=\"top\">", f);

    if (find_and_print_typedef(die))
        goto end;

    const char *name = dwarf_diename(&die->type);
    if (name)
        fprintf(f, "struct %s", name);
    else
        fputs("<I>unnamed struct</I>", f);

end:
    fputs("</TD></TR>\n", f);
}
