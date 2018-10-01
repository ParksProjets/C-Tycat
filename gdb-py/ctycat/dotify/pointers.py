"""

Dotify pointers.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import gdb

from ctycat.dotify.types import nodename
from ctycat.dotify.types import symvalue


def dotify_pointer_inline(value, data, f):
    "Dotify an inline pointer (e.g. in struct)."

    f.write(symvalue(value))

    try:
        newvalue = value.dereference()
    except:
        pass
    else:
        data.stack.append(newvalue)
        data.edges.append((nodename(value, data), nodename(newvalue)))



def dotify_pointer(value, data, f):
    "Dotify a pointer."

    f.write("%s [label=<" % nodename(value))
    dotify_pointer_inline(value, data, f)
    f.write(">];\n")
