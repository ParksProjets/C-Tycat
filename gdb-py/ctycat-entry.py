"""

Entry point for C-Tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from __future__ import print_function

import sys
import os.path as path


# Path to the C-Tycat library
CTYCAT_LIB = None



def has_ctycat(dirname):
    "Test if a directory has CTycat."

    return path.isfile(path.join(dirname, "ctycat", "__init__.py"))



def import_directory(dirname):
    "Try to import a directory."

    if not has_ctycat(dirname):
        return True

    sys.path.append(dirname)
    import ctycat



# Try importing C-Tycat
try:
    import ctycat
except ImportError:

    paths = []

    paths.append(path.abspath(path.dirname(__file__)))
    paths.append(path.abspath("~"))

    if CTYCAT_LIB:
        paths.append(CTYCAT_LIB)
        paths.append(path.join(CTYCAT_LIB, '..'))

    for p in paths:
        if not import_directory(p):
            break
    else:
        print("Can't find CTycat, please specify CTYCAT_LIB.")
