#
# GDB initalization file.
#
# Copyright (C) 2018, Guillaume Gonnet
# This project is under the MIT License

# You can use this file for auto-loading C-Tycat embedded extension into GDB. 
# You can put it in two places:
#   ~/.gdbinit  ->  C-Tycat embedded extension will be loaded every times
#   ./.gdbinit  ->  you must set "set auto-load local-gdbinit"

source ty-lib.py
