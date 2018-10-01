/*

Manage C-Tycat configuration.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/config.h"
#include "private/errors.h"

#include <string.h>


// Ensure that the C11 generic macro is disabled.
#ifdef ctycat_config_set
    #undef ctycat_config_set
#endif



// Get a configuation entry and check its type.
static int get_entry(const char *key, khiter_t *k, enum _Ctycat_Conftype type)
{
    *k = kh_get(_conf, _ctycat_confmap, key);

    if (*k == kh_end(_ctycat_confmap)) {
        _ctycat_error_info(key);
        _ctycat_report_warning(_CTY_WARN_CONF_KEY);
        return _CTY_WARN_CONF_KEY;
    }

    if (kh_value(_ctycat_confmap, *k).type != type) {
        _ctycat_error_info(key);
        _ctycat_report_warning(_CTY_WARN_CONF_TYPE);
        return _CTY_WARN_CONF_TYPE;
    }

    return _CTY_NO_ERROR;
}



// Set a configuration option (int value)
void ctycat_config_set(const char *key, int value)
{
    khint_t k;
    if (!get_entry(key, &k, _CTYCONF_INTEGER))
        *(int *)kh_value(_ctycat_confmap, k).value = value;
}


// Set a configuration option (string value)
void ctycat_config_setstr(const char *key, const char *value)
{
    khint_t k;
    if (get_entry(key, &k, _CTYCONF_STRING)) return;

    free(*(char **)kh_value(_ctycat_confmap, k).value);
    *(char **)kh_value(_ctycat_confmap, k).value = strdup(value);
}



// Get a configuration option (int value)
int ctycat_config_get(const char *key)
{
    khint_t k = kh_get(_conf, _ctycat_confmap, key);
    if (get_entry(key, &k, _CTYCONF_INTEGER))
        return -1;
    else
        return *(int *)kh_value(_ctycat_confmap, k).value;
}


// Get a configuration option (string value)
const char *ctycat_config_getstr(const char *key)
{
    khint_t k = kh_get(_conf, _ctycat_confmap, key);
    if (get_entry(key, &k, _CTYCONF_STRING))
        return "";
    else
        return *(char **)kh_value(_ctycat_confmap, k).value;
}
