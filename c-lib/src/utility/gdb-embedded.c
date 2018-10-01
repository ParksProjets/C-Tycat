/*

GDB embeded extension.

DO NOT MODIFY THIS FILE DIRECTLY.
This file is generated automatically by "gdb-convert.py".

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/config-values.h"

#if _CTYCONF_GDB_EXTENSION

#if _CTYCONF_GDB_EXTENSION_INLINE

asm("\
.pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n\
.byte 4\n\
.ascii \"gdb.inlined-script\\n\"\n\
.ascii \"class ty_cmd(gdb.Command):\\n\"\n\
.ascii \" \\\"\\\"\\\"\\n\"\n\
.ascii \" C-Tycat extension for GDB.\\n\"\n\
.ascii \" Run \\\"ty <varname>\\\" for tycat'ing a variable.\\n\"\n\
.ascii \" \\\"\\\"\\\"\\n\"\n\
.ascii \" def __init__(self):\\n\"\n\
.ascii \"  super(ty_cmd, self).__init__(\\\"ty\\\", gdb.COMMAND_DATA)\\n\"\n\
.ascii \" def invoke(self, arg, from_tty):\\n\"\n\
.ascii \"  s = gdb.selected_frame().find_sal()\\n\"\n\
.ascii \"  args = (s.symtab.filename, s.line, arg, arg)\\n\"\n\
.ascii \"  gdb.parse_and_eval('(void)_ctycat(\\\"%s\\\", %d, $pc, \\\"%s\\\", &%s)' % args)\\n\"\n\
.ascii \"ty_cmd()\\n\"\n\
.byte 0\n\
.popsection\n\
");

asm("\
.pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n\
.byte 4\n\
.ascii \"gdb.inlined-script\\n\"\n\
.ascii \"class ty_config_cmd(gdb.Command):\\n\"\n\
.ascii \" \\\"\\\"\\\"\\n\"\n\
.ascii \" Configure C-Tycat extension for GDB.\\n\"\n\
.ascii \" Run \\\"ty-config <option>\\\" to get the current value.\\n\"\n\
.ascii \" Run \\\"ty-config <option> <value>\\\" to set an option.\\n\"\n\
.ascii \" \\\"\\\"\\\"\\n\"\n\
.ascii \" def __init__(self):\\n\"\n\
.ascii \"  super(ty_config_cmd, self).__init__(\\\"ty-config\\\", gdb.COMMAND_DATA)\\n\"\n\
.ascii \" def invoke(self, arg, from_tty):\\n\"\n\
.ascii \"  print 'TODO'\\n\"\n\
.ascii \"ty_config_cmd()\\n\"\n\
.byte 0\n\
.popsection\n\
");

#else

asm("\
.pushsection \".debug_gdb_scripts\", \"MS\",@progbits,1\n\
.byte 1\n\
.asciz \"ty-embedded.py\"\n\
.popsection\n\
");

#endif

#endif
