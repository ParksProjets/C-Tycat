"""

Entry point for C-Tycat.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from ctycat.config import ConfigCommand
from ctycat.run import CtycatCommand


# Register GDB commands
ConfigCommand()
CtycatCommand()
