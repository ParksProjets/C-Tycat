/*

GDB embedded extension.

DO NOT MODIFY THIS FILE DIRECTLY.
This file is generated automatically by "gdb-convert.py".

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/config-values.h"

#if _CTYCONF_GDB_EXTENSION

#if _CTYCONF_GDB_EXTENSION_INLINE

//%file:src-ty.py

//%file:src-ty-config.py

#else

asm("\
.pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n\
.byte 1\n\
.asciz \"ty-libc.py\"\n\
.popsection\n\
");

#endif

#endif
