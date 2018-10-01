/*

Analyse a pointer.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"
#include "private/dotify.h"
#include "private/utility.h"



// Analyse a pointer.
void _ctyan_pointer(_Cty_Die *fdie, void *data)
{
    void *deref = *((void **)data);
    if (!_ctycat_ptr_valid(deref))
        return;

    Dwarf_Die die;
    Dwarf_Attribute attr;

    dwarf_attr(&fdie->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &die);

    Dwarf_Off offset = dwarf_dieoffset(&die);    
    int strzlen = _ctydot_string_len(offset, deref);

    // TODO: if strzlen > max_inline_string  ->  add a "string" die to stack.
    if (strzlen != -1) return;

    // Add the node into the stack so it will be analysed after
    kv_push(_NodeInfo, an.stack, (_NodeInfo){ offset, deref });
}
