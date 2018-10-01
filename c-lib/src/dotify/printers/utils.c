/*

Some useful functons for printing things. 

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Print the name of a node.
void _ctydot_nodename(void *data, int offset)
{
    fprintf(f, "s%x", (size_t)data);
    int shift = offset >> 2;

    if (offset & 1)
        fputs(":top", f);
    else if (offset & 2)
        fprintf(f, ":%d", shift);
}




// Print all edges in the stack.
void _ctydot_edges()
{
    for (size_t i = 0; i < kv_size(d->edges); i++) {
        _EdgeInfo *edge = &kv_A(d->edges, i);

        _ctydot_nodename(edge->start_addr, edge->start_offset);
        fputs(" -> ", f);
        _ctydot_nodename(edge->end_addr, edge->end_offset);
        fputs("\n", f);
    }

    kv_size(d->edges) = 0;
}
