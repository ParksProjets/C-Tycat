# 
# GDB command that will be embedded into the executable.
# 
# Copyright (C) 2018, Guillaume Gonnet
# This project is under the MIT License


class ty_cmd(gdb.Command):
    """
    C-Tycat extension for GDB.

    Run "ty <varname>" for tycat'ing a variable.
    """

    def __init__(self):
        super(ty_cmd, self).__init__("ty", gdb.COMMAND_DATA)


    def invoke(self, arg, from_tty):
        s = gdb.selected_frame().find_sal()
        args = (s.symtab.filename, s.line, arg, arg)
        gdb.parse_and_eval('(void)_ctycat("%s", %d, $pc, "%s", &%s)' % args)


ty_cmd()
