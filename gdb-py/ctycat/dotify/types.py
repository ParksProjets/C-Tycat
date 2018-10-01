"""

Dotify the memory.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import gdb


def nodename(value, data = None, nostruct = False):
    "Get the name of a node."

    address = int(str(value.address), 0)

    if data and data.struct_addr != 0:
        return "v%x:%d" % (data.struct_addr, address - data.struct_addr)
    elif not nostruct and value.type.code == gdb.TYPE_CODE_STRUCT:
        return "v%x:top" % address
    else:
        return "v%x" % address



def getvalue(addr, type):
    "Get a value at address."

    value = gdb.Value(addr).cast(type.pointer())
    return value.dereference()



def symvalue(value):
    "Get the value of a value."

    return "(<font color=\"royalblue\">%s</font>) %s" % (value.type, value)





# All subfunctions
from ctycat.dotify.pointers import dotify_pointer
from ctycat.dotify.structs import dotify_struct
from ctycat.dotify.values import dotify_value


def dotify_typed(value, data, f):
    "Dotify a value (call the appropriate function)."

    if value.address == 0:
        return

    code = value.type.code

    if code == gdb.TYPE_CODE_STRUCT:
        dotify_struct(value, data, f)
    elif code == gdb.TYPE_CODE_PTR:
        dotify_pointer(value, data, f)
    else:
        dotify_value(value, data, f)
