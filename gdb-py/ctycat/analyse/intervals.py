"""

Hash map with intervals.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

from ctycat.config import CONFIG_DICT as CONFIG



class Interval(object):
    "A memory interval."

    def __init__(self, start, end):
        self.start = start
        self.end = end




class HashedIntervals(object):
    "An hash map with intervals."

    def __init__(self):
        self.chunk_size = CONFIG["chunk-size"]
        self.map = dict()


    def find(self, addr):
        "Find the interval containing an address."

        index = addr // self.chunk_size
        interv = self.map.get(index, None)

        while interv:
            if interv.start <= addr <= interv.end:
                return interv
            interv = interv.next


    def add(self, start, end):
        "Add an address into the map."

        csize = self.chunk_size
        startidx = start // self.chunk_size
        endidx = start // self.chunk_size

        if startidx == endidx:
            return self._add_interval(startidx, Interval(start, end))

        self._add_interval(startidx, Interval(start, (startidx + 1) * csize))
        self._add_interval(endidx, Interval(endidx * csize, end))

        for idx in range(startidx + 1, endidx):
            self._add_interval(idx, Interval(idx * csize, (idx + 1) * csize))


    def _add_interval(self, index, interv):
        "Add an interval into the given chunk index."

        walker = self.map.get(index, None)
        if not walker:
            self.map[index] = interv

        elif walker.start <= interv.start:
            interv.next = walker
            self.map[index] = interv

        else:
            while walker.next:
                walker = walker.next

            pass # TODO
