/*

Analyse the data before dotify them.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"


// Data from analyse
struct _AnalyseData _ctycat_analysedata;



// Analyse the memory, before dotify it.
void _ctycat_analyse(Dwarf_Die *die, void *data)
{
    Dwarf_Off offset = dwarf_dieoffset(die);
    kv_push(_NodeInfo, an.stack, (_NodeInfo){ offset, data });

    _NodeInfo node;

    while (kv_size(an.stack) != 0) {
        node = kv_pop(an.stack);
        _ctyan_analyse(node.offset, node.data);
    }

    kv_destroy(an.stack);
}
