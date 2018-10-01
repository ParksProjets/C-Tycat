/*

Dotify a structure.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Dotify a struct member.
static void _ctydot_struct_field(Dwarf_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word offset;

    dwarf_attr(die, DW_AT_data_member_location, &attr);
    dwarf_formudata(&attr, &offset);

    unsigned int shift = (int)((char *)data - (char *)d->struct_addr + offset);

    fprintf(f, "  <TR><TD>%s</TD><TD port=\"%u\">", dwarf_diename(die), shift);


    Dwarf_Die ndie;
    dwarf_attr(die, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &ndie);

    _ctydot_dotify_inline(dwarf_dieoffset(&ndie), ((char *)data) + offset);

    fputs("</TD></TR>\n", f);
}




// Dotify the content of a structure.
void _ctydot_struct_inline(_Cty_Die *die, void *data)
{
    fputs("<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n", f);

    // TODO: remove port "top" if struct inline

    _ctydot_struct_name(die);


    Dwarf_Die child;
    if (dwarf_child(&die->type, &child))
        goto end;

    do {
        _ctydot_struct_field(&child, data);
    } while (dwarf_siblingof(&child, &child) == 0);

end:
    fputs("</TABLE>", f);
}




// Dotify a structure.
void _ctydot_struct(_Cty_Die *die, void *data)
{
    d->struct_addr = data;

    node_label(data);
    fputs("[label=<\n", f);

    _ctydot_struct_inline(die, data);
    fputs(">, shape=plaintext]\n", f);

    d->struct_addr = NULL;
}
