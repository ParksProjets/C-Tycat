/*

Load and unload C-Tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "ctycat.h"
#include "private/ctycat.h"
#include "private/config.h"
#include "private/errors.h"

#include <stdio.h>



// Load C-Tycat.
__attribute__((constructor)) static void _ctycat_load()
{
   _ctycat_load_default_config();
}



// Unload C-Tycat.
__attribute__((destructor)) static void _ctycat_unload()
{
    _ctycat_uninit();
    _ctycat_unload_config();

    printf("C-Tycat Unitialized!\n");
}
