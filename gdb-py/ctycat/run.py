"""

Run "ty" GDB command.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import gdb

from ctycat.dotify.dotify import dotify


def ctycat(varname):
    "Main function for C-Tycat"

    frame = gdb.selected_frame()
    block = frame.block()

    symbol, field = gdb.lookup_symbol(varname, block, gdb.SYMBOL_VAR_DOMAIN)
    if not symbol:
        raise gdb.GdbError("ty: variable '%s' not found" % varname)

    if not symbol.is_variable:
        raise gdb.GdbError("ty: symbol '%s' is not a variable" % symbol.name)

    # print "C-Tycat: %s of type %s" % (symbol.name, symbol.type)
    dotify(symbol.name, symbol.value(frame))




class CtycatCommand(gdb.Command):
    """
    C-Tycat extension for GDB.

    Run "ty <varname>" for tycat'ing a variable.
    """

    def __init__(self):
        super(CtycatCommand, self).__init__("ty", gdb.COMMAND_DATA)


    def invoke(self, arg, from_tty):
        "Invoke the command."

        args = gdb.string_to_argv(arg)
        if len(args) != 1:
            raise gdb.GdbError("ty: bad number of arguments")

        ctycat(args[0])


    def complete(self, text, word):
        "Auto-complete the command."

        return gdb.COMPLETE_SYMBOL
