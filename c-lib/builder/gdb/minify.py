"""

A simple minification module.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import re


# Regexps
_re_comments = re.compile("#.*$", re.M)
_re_empty = re.compile("\s*$", re.M)



def remove_comments_empty(code):
    "Remove one-line comments and empty lines."

    code = _re_comments.sub("", code)
    code = _re_empty.sub("", code)

    return code.strip()



def minify_indent(code):
    "Replace multi-spaces indents to one-space indents."

    stack = [0]
    lines = []

    def append(line, indent):
        nonlocal stack
        if indent > stack[-1]:
            stack.append(indent)
        else:
            stack = list(filter(lambda x: indent >= x, stack))

    for line in code.split("\n"):
        strip = line.lstrip()
        append(strip, len(line) - len(strip))
        lines.append((len(stack) - 1) * " " + strip)

    return "\n".join(lines)



def minify(code):
    "Minify a Python code."

    code = remove_comments_empty(code)
    code = minify_indent(code)

    return code
