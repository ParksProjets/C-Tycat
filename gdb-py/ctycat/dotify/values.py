"""

Dotify common values.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from ctycat.dotify.types import nodename
from ctycat.dotify.types import symvalue


def dotify_value_inline(value, data, f):
    "Dotify an inline value (e.g. in struct)."

    try:
        f.write(symvalue(value))
    except:
        f.write('error')



def dotify_value(value, data, f):
    "Dotify a common value."

    f.write("%s [label=<" % nodename(value))
    dotify_value_inline(value, data, f)
    f.write(">];\n")
