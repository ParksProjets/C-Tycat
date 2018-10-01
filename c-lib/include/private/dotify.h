/*

Convert the data into a .dot file.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_DOTIFY_H
#define _CTYCAT_DOTIFY_H

#include "config.h"
#include "analyse.h"
#include "khash.h"
#include "kvec.h"

#include <stdio.h>
#include <stdint.h>


// Information about an edge
typedef struct _EdgeInfo _EdgeInfo;
struct _EdgeInfo {
    void *start_addr;
    void *end_addr;
    int start_offset;
    int end_offset;
};



// Data from dotify
struct _DotifyData {
    void *struct_addr;
    kvec_t(_EdgeInfo) edges;
};


// Data from dotify for naive algorithm
struct _DotifyDataNaive {
    struct _DotifyData shared;
    kvec_t(_NodeInfo) stack;
};


// Data from dotify for analyse algorithm
struct _DotifyDataAnalyse {
    struct _DotifyData shared;
};



// Dot file to fill
extern FILE *_ctycat_dotfile;
#define f _ctycat_dotfile

// Data from dotify
extern struct _DotifyData *_ctycat_dotdata;
#define d _ctycat_dotdata
#define dn ((struct _DotifyDataNaive *)d)
#define da ((struct _DotifyDataAnalyse *)d)


// Hash function for address
#if UINTPTR_MAX == 0xffffffff
#define kh_addr_hash_func(key) kh_int_hash_func((khint32_t)key)
#else
#define kh_addr_hash_func(key) kh_int64_hash_func((khint64_t)key)
#endif

// Hash tables declaration
#define KHASH_SET_INIT_ADDR(name) \
    KHASH_INIT(name, void *, char, 0, kh_addr_hash_func, kh_int64_hash_equal)

KHASH_SET_INIT_ADDR(_addr)


// Print the label of the node
#define node_label(data) fprintf(f, "s%x ", (size_t)data);




// Get the offset of a node.
int _ctydot_node_offset(_Cty_Die *die, void *data);

// Get the name of the block address where the data is.
void *_ctydot_block_addr(void *data);

// Get the length of the string pointed (return -1 if not a string).
int _ctydot_string_len(Dwarf_Off offset, void *data);


// Dotify a type at the given address.
void _ctydot_dotify(Dwarf_Off offset, void *data);

// Dotify a structure.
void _ctydot_struct(_Cty_Die *die, void *data);

// Dotify a pointer.
void _ctydot_pointer(_Cty_Die *die, void *data);


// Dotify a type at the given address.
void _ctydot_dotify_inline(Dwarf_Off offset, void *data);

// Dotify the content of a structure.
void _ctydot_struct_inline(_Cty_Die *die, void *data);

// Dotify the content from a pointer.
void _ctydot_pointer_inline(_Cty_Die *die, void *data);


// Add the first node and edge for variable entry.
void _ctydot_entry(Dwarf_Die *die, void *data, const char *vname);

// Print the name of a node.
void _ctydot_nodename(void *data, int offset);

// Print the name of a structure.
void _ctydot_struct_name(_Cty_Die *die);

// Print all edges in the stack.
void _ctydot_edges();

// Print the type of a node.
void _ctydot_typename(_Cty_Die *die);

// Print the value of a node.
void _ctydot_value(_Cty_Die *die, void *data);


#endif // _CTYCAT_DOTIFY_H
