"""

C-Tycat GDB script for the C library.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from __future__ import print_function

import gdb
import textwrap
import os.path as path


# Configuration keys and types.
_config = {
    "enable-analyse": int,
    "malloced-arrays": int,
    "chunk-size": int,
    "max-array-size": int,
    "max-inline-string": int,
    "max-string": int,
    "save-png": int,
    "png-name": bool,
    "text-font": str,
    "rankdir": str
}


# Directories that potentially contain the dynamic library.
_folders = [
    ".",
    path.abspath(path.dirname(__file__)),
    path.expanduser("~"),
    "/usr/lib", "/usr/lib64"
]



class ty_cmd(gdb.Command):
    """
    Tycat a variable.

    Run "ty <varname>" for tycat'ing a variable.
    """

    def __init__(self):
        self.__doc__ = textwrap.dedent(self.__doc__).strip()
        super(ty_cmd, self).__init__("ty", gdb.COMMAND_DATA)


    def invoke(self, arg, from_tty):
        "Invoke the command."

        args = gdb.string_to_argv(arg)
        if len(args) != 1:
            raise gdb.GdbError("ty: bad number of arguments")

        s = gdb.selected_frame().find_sal()
        v = (s.symtab.filename, s.line, args[0], args[0])

        gdb.execute('call (void)_ctycat("%s", %d, $pc, "%s", &%s)' % v, False, True)


    def complete(self, text, word):
        "Auto-complete the command."

        return gdb.COMPLETE_SYMBOL



class ty_config_cmd(gdb.Command):
    """
    Configure C-Tycat extension for GDB.

    Run "ty-config <option>" to get an option value.
    Run "ty-config <option> <value>" to set an option.
    """

    def __init__(self):
        self.__doc__ = textwrap.dedent(self.__doc__).strip()
        super(ty_config_cmd, self).__init__("ty-config", gdb.COMMAND_DATA)


    def get_config(self, key):
        "Get a configuration option."

        if _config[key] is str:
            v = gdb.parse_and_eval('(char *)ctycat_config_getstr("%s")' % key)
            print('ctycat config: %s = "%s"' % (key, v.string()))
        else:
            v = gdb.parse_and_eval('(int)ctycat_config_get("%s")' % key)
            print('ctycat config: %s = %s' % (key, v))


    def set_config(self, key, val):
        "Set a configuration option."

        if _config[key] is str:
            gdb.parse_and_eval('(void)ctycat_config_setstr("%s", "%s")' % (key, val))
        else:
            try:
                val = int(val, 0)
            except:
                raise gdb.GdbError("ty-config: '%s' value must be an integer" % key)
            else:
                gdb.parse_and_eval('(void)ctycat_config_set("%s", %s)' % (key, val))


    def invoke(self, arg, from_tty):
        "Invoke the command."

        args = gdb.string_to_argv(arg)
        if not 0 < len(args) < 3:
            raise gdb.GdbError("ty-config: bad number of arguments")

        if args[0] not in _config:
            raise gdb.GdbError("ty-config: '%s' doesn't exist" % args[0])

        if len(args) == 1:
            self.get_config(*args)
        else:
            self.set_config(*args)


    def complete(self, text, word):
        "Auto-complete the command."

        if text.find(" ") != -1:
            return

        keys = _config.keys()
        l = len(text) - len(word)

        return map(lambda s: s[l:], filter(lambda s: s.startswith(text), keys))



# Try loading C-Tycat
try:
    gdb.parse_and_eval("_ctycat")
except:
    for folder in _folders:
        p = path.join(folder, "libctyd.so")
        if path.isfile(p):
            gdb.execute("set environment LD_PRELOAD %s" % p)
            loaded = True
            break
    else:
        loaded = False
else:
    loaded = True


# If C-Tycat is loaded: create the commands
if loaded:
    ty_cmd()
    ty_config_cmd()
else:
    print("ctycat error: can't load C library, ty command is disabled")
