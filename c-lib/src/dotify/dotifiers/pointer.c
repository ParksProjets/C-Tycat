/*

Dotify a pointer.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Return 1 if a pointer is valid.
static int is_pointer_valid(void *ptr)
{
    // TODO.

    return ptr != NULL;
}




// Dotify the content from a pointer.
void _ctydot_pointer_inline(_Cty_Die *fdie, void *data)
{
    _ctydot_value(fdie, data);

    void *deref = *((void **)data);
    if (!is_pointer_valid(deref))
        return;

    Dwarf_Die die;
    _Cty_Die fulldie;
    Dwarf_Attribute attr;

    dwarf_attr(&fdie->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &die);

    Dwarf_Off offset = dwarf_dieoffset(&die);    
    int strzlen = _ctydot_string_len(offset, deref);

    _ctycat_format_die(dwarf_dieoffset(&die), &fulldie);



    // TODO: if strzlen > max_inline_string  ->  add a "string" die to stack.
    if (strzlen != -1) return;

    // Add the node into the stack so it will be dotify after
    if (!c.analyse)
        kv_push(_NodeInfo, dn->stack, (_NodeInfo){ offset, deref });

    // Append the edge into the edge list
    kv_push(_EdgeInfo, d->edges, (_EdgeInfo){
        _ctydot_block_addr(data), deref /* FIX ME */,
        _ctydot_node_offset(NULL, data), _ctydot_node_offset(&fulldie, deref)
    });
}




// Dotify a pointer.
void _ctydot_pointer(_Cty_Die *die, void *data)
{
    node_label(data);

    fputs("[label=<", f);
    _ctydot_pointer_inline(die, data);
    fputs(">]\n", f);
}
