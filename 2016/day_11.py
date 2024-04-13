import datetime
from collections import deque, Counter
import re
from itertools import chain, combinations

begin_time = datetime.datetime.now()

floors = []


def is_valid_transition(floor):
    return len(set(type for _, type in floor)) < 2 or \
           all((obj, 'generator') in floor
               for (obj, type) in floor
               if type == 'microchip')


def next_states(moves, cur_level, floors):
    for move in chain(combinations(floors[cur_level], 2), combinations(floors[cur_level], 1)):
        for direction in [-1, 1]:
            nxt_level = cur_level + direction
            if 0 <= nxt_level < len(floors):
                next_floors = floors.copy()
                next_floors[nxt_level] = next_floors[nxt_level] | set(move)
                next_floors[cur_level] = next_floors[cur_level] - set(move)

                if is_valid_transition(next_floors[cur_level]) and is_valid_transition(next_floors[nxt_level]):
                    yield moves + 1, nxt_level, next_floors


# found next here, w/o runs very long but why this works ¯\_(ツ )_/¯
# https://eddmann.com/posts/advent-of-code-2016-day-11-radioisotope-thermoelectric-generators/
def count_floor_objects(state):
    _, elevator, floors = state
    return elevator, tuple(tuple(Counter(type for _, type in floor).most_common()) for floor in floors)


def bfs(floors):
    seen = set()
    queue = deque([(0, 0, floors)])

    while queue:
        state = queue.popleft()
        moves, level, floors = state

        if all(len(items) == 0 for items in floors[0:-1]):
            return moves

        for next_state in next_states(moves, level, floors):
            if (key := count_floor_objects(next_state)) not in seen:
                seen.add(key)
                queue.append(next_state)


with open('./inputs/11.txt') as f:
    while line := f.readline().rstrip():
        floors.append(set(re.findall(r'(\w+)(?:-compatible)? (microchip|generator)', line)))

print(bfs(floors))

floors[0] = floors[0] | {('elerium', 'generator'), ('elerium', 'microchip'), ('dilithium', 'generator'),
                         ('dilithium', 'microchip')}

print(bfs(floors))
print(datetime.datetime.now() - begin_time)