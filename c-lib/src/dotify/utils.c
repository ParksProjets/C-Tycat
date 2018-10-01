/*

Some useful functions.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"


//
static int offset_analyse(void *data)
{
    // TODO
    return 0;
}



// Get the offset of a node.
int _ctydot_node_offset(_Cty_Die *die, void *data)
{
    if (!die) {
        if (d->struct_addr)
            return (int)((size_t)data - (size_t)d->struct_addr) << 2 | 2;
        else
            return 0;
    }

    else {
        int type = dwarf_tag(&die->type);
        int offset = (type == DW_TAG_structure_type);

        if (!c.analyse)
            return offset; // no analyse -> assume not ptr to struct member
        else
            return offset | (offset_analyse(data) << 2) | 2;
    }
}



// Get the name of the block address where the data is.
void *_ctydot_block_addr(void *data)
{
    // TODO: if c.analyse

    return d->struct_addr ? d->struct_addr : data;
}




// Get the length of the string pointed (return -1 if not a string).
int _ctydot_string_len(Dwarf_Off offset, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;
    
    _Cty_Die die;
    _ctycat_format_die(offset, &die);

    if (!dwarf_hasattr(&die.type, DW_AT_encoding))
        return -1;

    dwarf_attr(&die.type, DW_AT_encoding, &attr);
    dwarf_formudata(&attr, &word);

    if (word != DW_ATE_signed_char)
        return -1;

    return strlen(data);
}
