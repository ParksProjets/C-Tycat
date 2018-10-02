/*

Private part of C-Tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_PRIVATE_H
#define _CTYCAT_PRIVATE_H

#include "khash.h"

#include <gelf.h>
#include <dwarf.h>
#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>


// Hashmap of CU dies
KHASH_MAP_INIT_STR(_cuhash, int)
extern khash_t(_cuhash) *_ctycat_cumap;


// Is C-Tycat initialized?
extern _Bool _ctycat_initialized;

// Have we already reported an error?
extern int _ctycat_error_reported;

// Dwarf and Dwfl containers
extern Dwarf *_ctycat_dwarf;
extern Dwfl *_ctycat_dwfl;

// Number of the current C-Tycat (starts at one).
extern int _ctycat_number;



// Initialize Dwarf module used by C-Tycat.
int _ctycat_init_dward();

// Uninitialize C-Tycat.
void _ctycat_uninit();

// Convert the data into a .dot file.
void _ctycat_dotify(Dwarf_Die *typedie, void *data, const char *vname);


#endif // _CTYCAT_PRIVATE_H
