/*

Dotify an array.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Analyse an array from a pointer.
void _ctydot_ptrarray(_Cty_Die *die, void *data, size_t size)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;

    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    Dwarf_Off off = dwarf_dieoffset(&die->qualifiers);


    // Dotify each element of the array.
    char *end = (char *)data + size;
    char *pos = data;

    for (; pos < end; pos += word)
        _ctydot_dotify_inline(off, pos);
}



// Dotify the content from an array.
void _ctydot_array_inline(_Cty_Die *fdie, void *data)
{
   // TODO.
}



// Dotify an array.
void _ctydot_array(_Cty_Die *die, void *data)
{
    // TODO.
}
