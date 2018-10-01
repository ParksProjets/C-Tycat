# 
# GDB command for modify C-Tycat configuration.
# 
# Copyright (C) 2018, Guillaume Gonnet
# This project is under the MIT License


class ty_config_cmd(gdb.Command):
    """
    Configure C-Tycat extension for GDB.

    Run "ty-config <option>" to get the current value.
    Run "ty-config <option> <value>" to set an option.
    """

    def __init__(self):
        super(ty_config_cmd, self).__init__("ty-config", gdb.COMMAND_DATA)


    def invoke(self, arg, from_tty):
        print 'TODO'


ty_config_cmd()
