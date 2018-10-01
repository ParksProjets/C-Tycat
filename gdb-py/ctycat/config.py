"""

Manage C-Tycat configuration.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from __future__ import print_function
import gdb


# Store the whole configuration
CONFIG_DICT = {}


# Default configuration
CONFIG_DICT["max-array-size"] = 20

CONFIG_DICT["max-string"] = 20
CONFIG_DICT["max-inline-string"] = 20

CONFIG_DICT["malloced-array"] = 1

CONFIG_DICT["show-malloced"] = 0
CONFIG_DICT["show-mmaped"] = 0

CONFIG_DICT["chunk-size"] = 1024



def config_set(key, value):
    "Set a configuration option."

    if isinstance(value, str):
        try:
            value = int(value, 0)
        except ValueError:
            raise gdb.GdbError("Configuration value must be an integer")

    if key not in CONFIG_DICT:
        raise gdb.GdbError("Configuration key '%s' doesn't exist" % key)

    CONFIG_DICT[key] = value



def config_get(key):
    "Get a configuration option."

    if key not in CONFIG_DICT:
        raise gdb.GdbError("Configuration key '%s' doesn't exist" % key)

    return CONFIG_DICT[key]




class ConfigCommand(gdb.Command):
    """
    Configure C-Tycat extension for GDB.

    Run "ty-config <option>" to get the current value.
    Run "ty-config <option> <value>" to set an option.
    """

    def __init__(self):
        super(ConfigCommand, self).__init__("ty-config", gdb.COMMAND_DATA)


    def invoke(self, arg, from_tty):
        "Invoke the command."

        args = gdb.string_to_argv(arg)
        if not 0 < len(args) < 3:
            raise gdb.GdbError("ty-config: bad number of arguments")

        key = args[0]
        if len(args) == 1:
            print("ctycat config: %s = %d" % (key, config_get(key)))
        else:
            config_set(key, args[1])


    def complete(self, text, word):
        "Auto-complete the command."

        if text.find(" ") != -1:
            return

        keys = CONFIG_DICT.keys()
        l = len(text) - len(word)

        return map(lambda s: s[l:], filter(lambda s: s.startswith(text), keys))
