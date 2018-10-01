"""

Static data for storing things.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""


class NaiveDumpData(object):
    "Data for naive dump."

    def __init__(self):
        self.seen = set()
        self.stack = []

        self.struct_addr = 0
        self.edges = []
