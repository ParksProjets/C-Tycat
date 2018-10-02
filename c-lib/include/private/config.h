/*

C-Tycat configuration.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#ifndef _CTYCAT_CONFIG_H
#define _CTYCAT_CONFIG_H

#include "khash.h"

#include <stdint.h>
#include <setjmp.h>


// All config value types
enum _Ctycat_Conftype {
    _CTYCONF_INTEGER = 0,
    _CTYCONF_STRING = 1
};


// An entry in the config
struct _ConfigEntry {
    enum _Ctycat_Conftype type;
    void *value;
};


// Configuration hash map
KHASH_MAP_INIT_STR(_conf, struct _ConfigEntry)
extern khash_t(_conf) *_ctycat_confmap;



// Config file tokens.
enum _Cty_ConfTokens {
    _CTYTOK_WORD = 1,
    _CTYTOK_INTEGER = 2,
    _CTYTOK_STRING = 4,
    _CTYTOK_BOOLEAN = 8,
    _CTYTOK_ASSIGN = 16,
    _CTYTOK_END = 32
};


// Additional values parsed by the lexer.
extern int _ctyconf_interger;
extern char _ctyconf_string[50];
extern char _ctyconf_word[40];

// Cursor position.
extern char *_ctyconf_lastp;
extern char *_ctyconf_linep;
extern char *_ctyconf_line;

// Error data.
extern _Bool _ctyconf_error_reported;
extern jmp_buf _ctyconf_jmp;
extern int _ctyconf_lineno;


// Get the next token from the lexer.
enum _Cty_ConfTokens _ctyconf_next();

// Print a parser error.
void _ctyconf_error(const char *msg, ...);

// Print a token error.
void _ctyconf_error_token(enum _Cty_ConfTokens token);


// Load default ctycat configuration.
void _ctycat_load_default_config();

// Unload C-Tycat configuration.
void _ctycat_unload_config();



// All configuration parameters.
struct _Config {
    int analyse;

    int chunk_size;
    int max_array_size;
    int max_string;
    int max_inline_string;
    int malloced_array;

    int save_dot;
    int save_png;
    char *dot_name;
    char *png_name;

    char *rankdir;
    char *font;
};


// Store current configuraion.
extern struct _Config _ctycat_conf;

// Shortname configuration.
#define c _ctycat_conf


#endif // _CTYCAT_CONFIG_H
