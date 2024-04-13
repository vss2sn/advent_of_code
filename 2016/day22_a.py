import datetime
import itertools
import re
from dataclasses import dataclass

begin_time = datetime.datetime.now()

@dataclass
class Node:
    pos: tuple
    size: int
    used: int

    def free(self):
        return self.size - self.used

    def viable_pair(self, other):
        if self.pos == other.pos:
            return False
        return (0 < other.used <= self.free()) or \
               (0 < self.used <= other.free())


def count_viable(nodes):
    v = 0
    for a, b in itertools.combinations(nodes.values(), 2):
        if a.viable_pair(b):
            v += 1
    return v


nodes = dict()
with open('./inputs/22.txt') as f:
    while line := f.readline().rstrip():
        if line.startswith('/dev/grid'):
            x, y, siz, used, *_ = list(map(int, re.findall(r'\d+', line)))
            p = tuple((x, y))
            n = Node(p, siz, used)
            nodes[p] = n

print(count_viable(nodes))
print(datetime.datetime.now() - begin_time)