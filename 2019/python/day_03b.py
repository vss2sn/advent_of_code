def GetWireCoordinatesList(input):
    """ mark all position covered by a wire """
    init_x = init_y = 0
    wire = list()
    for inst in input:
        if inst[0] == "R":
            for inc in range(1, int(inst[1:]) + 1):
                init_x += 1
                wire.append((init_x, init_y))
        elif inst[0] == "U":
            for inc in range(1, int(inst[1:]) + 1):
                init_y += 1
                wire.append((init_x, init_y))
        elif inst[0] == "L":
            for inc in range(1, int(inst[1:]) + 1):
                init_x -= 1
                wire.append((init_x, init_y))
        elif inst[0] == "D":
            for inc in range(1, int(inst[1:]) + 1):
                init_y -= 1
                wire.append((init_x, init_y))
    return wire

import sys
input1 = sys.stdin.readline().strip("\n").split(",")
input2 = sys.stdin.readline().strip("\n").split(",")

listwire1 = GetWireCoordinatesList(input1)
listwire2 = GetWireCoordinatesList(input2)
setwire2 = set(listwire2)  # to speed lookup

distance = 1E9
for dist1, coor in enumerate(listwire1):
    if coor in setwire2:
        distance = min(distance, dist1 + listwire2.index(coor))  # List.index is costly but happends only when there is an intersection, only 40 times for my input
distance += 2  # the two lists do not contain (0,0) to save an if statement in the find intersection step, must add
print(distance)
