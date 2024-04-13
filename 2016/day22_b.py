import datetime
import re
from collections import deque

def vector_add(pos, d):
    return pos[0] + d[0], pos[1] + d[1]

def adjacents(pos):
    for npos in [vector_add(pos, d) for d in [(0, 1), (0, -1), (-1, 0), (1, 0)]]:
        nx, ny = npos
        if 0 <= ny < len(sizes) and 0 <= nx < len(sizes[ny]):
            yield nx, ny

def bfs(start, end):
    q = deque([(*start, 0, [])])
    seen = set()
    while q:
        px, py, steps, path = q.popleft()
        fpos = (px, py)

        if fpos == end:
            return steps, path

        for ax, ay in adjacents((px, py)):
            if sizes[py][px] >= useds[ay][ax] and tuple([ax, ay]) not in seen:
                seen.add((ax, ay))
                q.append((ax, ay, steps+1, path + [(ax, ay)]))

with open('./inputs/22.txt') as f:
    sizes = []
    useds = []
    for line in f:
        if line.startswith('/dev/grid'):
            x, y, siz, used, *_ = list(map(int, re.findall(r'\d+', line)))
            if len(sizes) <= y:
                sizes.append([])
                useds.append([])

            sizes[y].append(siz)
            useds[y].append(used)

    empty_pos = [(x, y) for y, row in enumerate(useds) for x, used in enumerate(row) if used == 0][0]
    goal_pos = (len(sizes[0]) - 1, 0)

steps, path = bfs(empty_pos, goal_pos)

for px, py in path:
    ex, ey = empty_pos
    useds[ey][ex] = useds[py][px]
    empty_pos = (px, py)

goal_pos = path[-2]
steps += (5 * goal_pos[0])

print(steps)
