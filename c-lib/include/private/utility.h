/*

Utility module: a lot of useful functions.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_UTILITY_H
#define _CTYCAT_UTILITY_H

#include <stdio.h>


// Open the DOT file (and get its name).
const char *_ctycat_open_dot();

// Print and HTML escape a text.
void _ctycat_print_escaped(const char *text);

// Convert a DOT file into a PNG image.
void _ctycat_dot_to_png(const char *dotname);


// Is a pointer valid.
_Bool _ctycat_ptr_valid(void *ptr);


// Return 1 if ptr has been malloc'ed.
_Bool _ctycat_is_malloced(void *ptr);

// Get the malloc'ed array size.
size_t _ctycat_malloced_size(void *ptr);


#endif // _CTYCAT_UTILITY_H
