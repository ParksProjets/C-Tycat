"""

Embed Python scripts into a C code.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import os.path as path
import re

from minify import minify


# Regexps
_re_split = re.compile("//%file:(.*)$", re.M)



def convert(filename):
    "Convert a Python script into C code."

    with open(filename) as file:
        code = minify(file.read())

    lines = [
        '.pushsection \\".debug_gdb_scripts\\", \\"MS\\",@progbits,1\\n\\',
        '.byte 4\\n\\',
        '.ascii \\"gdb.inlined-script\\\\n\\"\\n\\'
    ]

    for line in code.split("\n"):
        line = line.replace('"', '\\\\\\"')
        lines.append('.ascii \\"%s\\\\n\\"\\n\\' % line)

    lines += [
        '.byte 0\\n\\',
        '.popsection\\n\\'
    ]

    return 'asm("\\\n%s\n");' % "\n".join(lines)



def read_c(filename, here):
    "Read and split the C source code."

    with open(filename) as file:
        code = file.read()

    split = _re_split.split(code)
    for i in range(1, len(split), 2):
        split[i] = convert(path.join(here, split[i]))

    return "".join(split)



def main():
    "Entry point of this script."

    here = path.dirname(__file__)
    root = path.join(here, "../..")

    code = read_c(path.join(here, "base-source.c"), here)

    with open(path.join(root, "src/utility/gdb-embedded.c"), "w") as file:
        file.write(code)


if __name__ == "__main__":
    config = main()
