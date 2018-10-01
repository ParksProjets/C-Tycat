"""

Dotify the memory.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import gdb

from tempfile import NamedTemporaryFile
from os import system
from time import sleep

from ctycat.dotify.data import NaiveDumpData
from ctycat.dotify.types import nodename
from ctycat.dotify.types import dotify_typed
from ctycat.dotify.types import getvalue


def dotify_entry(vname, value, f):
    "Dotify the entry symbol."

    f.write("entry [label=\"%s\", fontcolor=\"red3\", color=\"red\"]\n" % vname)
    f.write("entry -> %s [color=\"red2\", dir=none]\n\n" % nodename(value))



def write_edges(data, f):
    "Write edges into the file."

    for (v1, v2) in data.edges:
        f.write("%s -> %s\n" % (v1, v2))

    data.edges = []



def dump_memory(vname, value, f):
    "Dump the whole memory from a variable."

    data = NaiveDumpData()

    f.write("digraph {\n")
    f.write("node [fontname=Arial, shape=record]\n")
    f.write("rankdir=LR\n\n")

    dotify_entry(vname, value, f)
    data.stack.append(value)

    while len(data.stack):
        dotify_typed(data.stack.pop(), data, f)
        write_edges(data, f)

    f.write("}\n")



def dotify(vname, value):
    "Dotify the memory."

    file = open("test.dot", "w")
    # file = NamedTemporaryFile(mode="w", delete=False)
    dump_memory(vname, value, file)
    file.close()

    system("dot -Tpng %s -o %s.png" % (file.name, file.name))
    system("tycat %s.png" % file.name)
    sleep(1)
