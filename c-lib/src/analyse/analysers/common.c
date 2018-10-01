/*

Analyse common types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"



// Analyse a base type.
void _ctyan_basetype(_Cty_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;

    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    _ctycat_add_interval((uintptr_t)data, (uintptr_t)data + word);
}




// Analyse a type at the given address.
void _ctyan_analyse(Dwarf_Off offset, void *data)
{
    if (_ctycat_get_interval(data))
        return; // The node has already been analysed.

    _Cty_Die die;
    _ctycat_format_die(offset, &die);

    size_t size;
    if (c.malloced_array && (size = _ctycat_malloced_size(data)) > 0)
        return _ctyan_ptrarray(&die, data, size);


    int type = dwarf_tag(&die.type);

    if (type == DW_TAG_structure_type)
        _ctyan_struct(&die, data);

    else if (type == DW_TAG_pointer_type)
        _ctyan_pointer(&die, data);

    _ctyan_basetype(&die, data);


    // TODO: array, union, enum, function?, complex, typedef
}




// Analyse a type at the given address.
void _ctyan_analyse_inline(Dwarf_Off offset, void *data)
{
    if (_ctycat_get_interval(data))
        return; // The node has already been analysed.

    _Cty_Die die;
    _ctycat_format_die(offset, &die);


    int type = dwarf_tag(&die.type);

    if (type == DW_TAG_structure_type)
        _ctyan_struct(&die, data);

    else if (type == DW_TAG_pointer_type)
        _ctyan_pointer(&die, data);
}
