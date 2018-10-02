/*

Default C-Tycat configuration.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/config.h"
#include "private/config-values.h"

#include <string.h>


// Configuration hash map
khash_t(_conf) *_ctycat_confmap;
struct _Config _ctycat_conf;

// Shortcut for configuration storages
#define c _ctycat_conf
#define map _ctycat_confmap
#define _(name) _CTYCONF_ ## name



// Set an int value.
static void setint(const char *key, int value, int *storage)
{
    int absent;
    khiter_t k = kh_put(_conf, map, key, &absent);

    kh_value(map, k) = (struct _ConfigEntry){ _CTYCONF_INTEGER, storage };
    *storage = value;
}


// Set a string value.
static void setstr(const char *key, char *value, char **storage)
{
    int absent;
    khiter_t k = kh_put(_conf, map, key, &absent);

    kh_value(map, k) = (struct _ConfigEntry){ _CTYCONF_STRING, storage };
    *storage = strdup(value);
}



// Unload C-Tycat configuration.
void _ctycat_unload_config()
{
    khiter_t k = kh_begin(map);

    for (; k != kh_end(map); k++) {
        if (kh_exist(map, k) && kh_value(map, k).type == _CTYCONF_STRING)
            free(*(char **)kh_value(map, k).value);
    }

    kh_destroy(_conf, map);
}



// Load default C-Tycat configuration.
void _ctycat_load_default_config()
{
    _ctycat_confmap = kh_init(_conf);


    // Max size of displayed array.
    setint("enable-analyse", _(ENABLE_ANALYSE), &c.analyse);

    // Size of a chunk containing intervals.
    setint("chunk-size", _(CHUNK_SIZE), &c.chunk_size);


    // Max size of displayed array.
    setint("max-array-size", _(MAX_ARRAY_SIZE), &c.max_array_size);

    // Max length of displayed string.
    setint("max-string", 50, &c.max_string);

    // Max length of inlined strings.
    setint("max-inline-string", _(MAX_INLINE_STRING), &c.max_inline_string);


    // Tycat malloc'ed array.
    setint("malloced-arrays", _(MALLOCED_ARRAYS), &c.malloced_array);


    // Save the generated DOT file in another place than '/tmp'.
    setint("save-dot", _(SAVE_DOT), &c.save_dot);

    // Name of the DOT file to save (if 'save-dot' is true).
    setstr("dot-name", _(DOT_NAME), &c.dot_name);


    // Save the generated PNG file in another place than '/tmp'.
    setint("save-png", _(SAVE_PNG), &c.save_png);

    // Name of the PNG file to save (if 'save-png' is true).
    setstr("png-name", _(PNG_NAME), &c.png_name);


    // Font of the text in the graph.
    setstr("text-font", _(TEXT_FONT), &c.font);

    // Global direction of the graph (Left-Right or Top-Bottom).
    setstr("rankdir", _(RANKDIR), &c.rankdir);
}
