/*

C-Tycat main header.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_H
#define _CTYCAT_H


// C-Tycat version.
#define CTYCAT_VERSION "1.0"


// You can disable C-Tycat by defining the macro 'CTYCAT_DISABLED'.
#ifndef CTYCAT_DISABLED


// Read a configuration file.
void ctycat_read_config(const char *filename);

// Set a configuration option.
void ctycat_config_set(const char *key, int value);
void ctycat_config_setstr(const char *key, const char *value);

// Get a configuration option.
int ctycat_config_get(const char *key);
const char *ctycat_config_getstr(const char *key);


// C11 generic config set.
#if __STDC_VERSION__ >= 201112L
#define ctycat_config_set(key, val) _Generic((0, val), \
        int: ctycat_config_set, \
        char *: ctycat_config_setstr, \
        const char *: ctycat_config_setstr \
    )(key, val)
#endif



// Raw function for C tycat.
void _ctycat(const char *fname, int line, void *addr, const char *vname, void *data);

// Tycat a variable
#define ctycat(var) do { __label__ l; l: _ctycat(__FILE__, __LINE__, &&l, #var, \
    &var); } while(0)


#else


// If C-Tycat is disabled, mock previous functions.
#define ctycat_read_config(filename) ((void) 0)
#define ctycat_config_set(key, value) ((void) 0)
#define ctycat_config_setstr(key, value) ((void) 0)
#define ctycat_config_get(key) ((void) 0)
#define ctycat_config_getstr(key) ((void) 0)

#define ctycat(var) ((void) 0)


#endif // CTYCAT_DISABLED
#endif // _CTYCAT_H
