/*

Analyse an array.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"
#include "private/dotify.h"
#include "private/utility.h"



// Analyse an array from a pointer.
void _ctyan_ptrarray(_Cty_Die *die, void *data, size_t size)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;

    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    Dwarf_Off off = dwarf_dieoffset(&die->qualifiers);


    // Analyse each element of the array.
    char *end = (char *)data + size;
    char *pos = data;
    
    for (; pos < end; pos += word)
        _ctyan_analyse_inline(off, pos);

    // Add the whole array into the interval container.
    // _ctycat_add_interval((uintptr_t)data, (uintptr_t)end, off);
}
