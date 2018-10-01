/*

Analyse the data before dotify them.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_ANALYSE_H
#define _CTYCAT_ANALYSE_H

#include "config.h"
#include "khash.h"
#include "kvec.h"

#include <dwarf.h>
#include <elfutils/libdw.h>


// Hash address key
#define kh_address_hash_func(key) (size_t)(key)

// Map with address as key
#define KHASH_MAP_INIT_ADDR(name, khval_t) \
    KHASH_INIT(name, size_t, khval_t, 1, kh_address_hash_func, kh_int_hash_equal)



// Helper for dealing with dies
typedef struct _FormatedDie _Cty_Die;
struct _FormatedDie {
    Dwarf_Die qualifiers;
    Dwarf_Die type;
};


// Information about a node
typedef struct _NodeInfo _NodeInfo;
struct _NodeInfo {
    Dwarf_Off offset;
    void *data;
};


// Data from analyse
struct _AnalyseData {
    kvec_t(_NodeInfo) stack;
};

// Data from analyse
extern struct _AnalyseData _ctycat_analysedata;
#define an _ctycat_analysedata



// A chunck containing some intervals.
struct _Chunck {
    struct _Interval *top;
};


// An interval, in a linked list.
struct _Interval {
    struct _Interval *next;
    int ref;
    size_t start;
    size_t end;
};


// Hash map containing chunks
KHASH_MAP_INIT_ADDR(_chunks, struct _Chunck)
extern khash_t(_chunks) *_ctycat_chunks;



// Format a Libdw die.
void _ctycat_format_die(Dwarf_Off offset, _Cty_Die *die);

// Analyse the memory, before dotify it.
void _ctycat_analyse(Dwarf_Die *die, void *data);


// Get the interval containing an address
struct _Interval *_ctycat_get_interval(size_t addr);

// Add a new interval into the container.
void _ctycat_add_interval(size_t start, size_t end);


// Analyse a type at the given address.
void _ctyan_analyse(Dwarf_Off offset, void *data);

// Analyse a type at the given address.
void _ctyan_analyse_inline(Dwarf_Off offset, void *data);


// Analyse a base type.
void _ctyan_basetype(_Cty_Die *die, void *data);

// Analyse an array from a pointer.
void _ctyan_ptrarray(_Cty_Die *fdie, void *data, size_t size);


#endif // _CTYCAT_ANALYSE_H
