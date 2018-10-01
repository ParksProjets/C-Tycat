"""

Dotify structures.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import gdb

from ctycat.dotify.types import nodename
from ctycat.dotify.types import symvalue
from ctycat.dotify.types import getvalue

from ctycat.dotify.pointers import dotify_pointer_inline
from ctycat.dotify.values import dotify_value_inline


def dotify_struct_mem(field, parent, data, f):
    "Dotify a structure member."

    f.write("  <TR><TD>%s</TD><TD PORT=\"%s\">" % (field.name, field.bitpos // 8))

    code = field.type.code
    value = getvalue(int(str(parent.address), 0) + (field.bitpos // 8), field.type)

    if code == gdb.TYPE_CODE_PTR:
        dotify_pointer_inline(value, data, f)
    else:
        dotify_value_inline(value, data, f)

    f.write("</TD></TR>\n")



def dotify_inline_struct(value, data, f):
    "Dotify an inline structure."

    f.write("<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n")

    # TODO: test if structure named
    f.write("  <TR><TD COLSPAN=\"2\" bgcolor=\"gray82\" port=\"top\">%s"
        "</TD></TR>\n" % value.type)

    for field in value.type.fields():
        dotify_struct_mem(field, value, data, f)

    f.write("</TABLE>")



def dotify_struct(value, data, f):
    "Dotify a structure."

    f.write("%s [label=<\n" % nodename(value, None, True))
    data.struct_addr = int(str(value.address), 0)

    dotify_inline_struct(value, data, f)

    data.struct_addr = 0
    f.write(">, shape=plaintext]\n")
