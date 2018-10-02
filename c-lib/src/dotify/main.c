/*

Main loops of dotify engine.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"
#include "private/ctycat.h"
#include "private/utility.h"
#include "private/errors.h"


// Dot file to fill
FILE *_ctycat_dotfile;

// Data from dotify
struct _DotifyData *_ctycat_dotdata;



// Initialize dotify common data
static void dotify_init()
{
    d->struct_addr = 0;
    kv_init(d->edges);
}


// Uninitialize dotify common data
static void dotify_uninit()
{
    kv_destroy(d->edges);
}



// Dotify the memory without any pre-analyses.
void _ctycat_dotify_naive(Dwarf_Die *die, void *data, const char *vname)
{
    struct _DotifyDataNaive dotdata;
    d = (struct _DotifyData *)&dotdata;

    dotify_init();

    khash_t(_addr) *seen = kh_init(_addr);
    kv_init(dn->stack);

    Dwarf_Off offset = dwarf_dieoffset(die);
    kv_push(_NodeInfo, dn->stack, (_NodeInfo){ offset, data });

    _NodeInfo node;
    int dummy;

    _ctydot_entry(die, data, vname);

    while (kv_size(dn->stack) != 0) {
        node = kv_pop(dn->stack);

        if (kh_get(_addr, seen, node.data) != kh_end(seen))
            continue;

        _ctydot_dotify(node.offset, node.data);
        _ctydot_edges();

        kh_put(_addr, seen, node.data, &dummy);
    }

    kh_destroy(_addr, seen);
    kv_destroy(dn->stack);

    dotify_uninit();
}




// Dotify the memory after a complete analyse.
void _ctycat_dotify_analysed()
{
    // TODO.

    _ctycat_report_error(_CTY_FATAL_ERROR);
}




// Add the first node and edge for variable entry
void _ctydot_entry(Dwarf_Die *die, void *data, const char *vname)
{
    fputs("entry [label=\"", f);
    _ctycat_print_escaped(vname);
    fputs("\", fontcolor=\"red3\", color=\"red\"]\n", f);

    _Cty_Die fdie;
    _ctycat_format_die(dwarf_dieoffset(die), &fdie);

    fputs("entry -> ", f);
    _ctydot_nodename(data, _ctydot_node_offset(&fdie, data));

    fputs(" [color=\"red2\", dir=none]\n\n", f);
}



// Entry point of the dotify system.
void _ctycat_dotify_entry(Dwarf_Die *die, void *data, const char *vname)
{
    fputs("digraph {\n", f);
    fprintf(f, "node [fontname=\"%s\", shape=record]\n", c.font);
    fprintf(f, "rankdir=%s\n\n", c.rankdir);

    if (c.analyse)
        _ctycat_dotify_analysed();
    else
        _ctycat_dotify_naive(die, data, vname);

    fputs("}\n", f);
}



// Convert the data into a .dot file.
void _ctycat_dotify(Dwarf_Die *typedie, void *data, const char *vname)
{
    const char *dotname = _ctycat_open_dot();
    if (!dotname) return;

    _ctycat_dotify_entry(typedie, data, vname);
    fclose(f);

    _ctycat_dot_to_png(dotname);
}
