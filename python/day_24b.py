#!/usr/bin/python3

def getNeighbourCount(coords, isWhiteUp):
    neighbour_increments = [(1,1), (1,-1), (-1,1), (-1,-1), (2,0), (-2,0)]
    count = 0
    for neighbour_increment in neighbour_increments:
        if (coords[0] + neighbour_increment[0], coords[1] + neighbour_increment[1]) in isWhiteUp and \
            not isWhiteUp[coords[0] + neighbour_increment[0], coords[1] + neighbour_increment[1]]:
            count = count + 1
    return count

def MakeArtLive(isWhiteUp):
    neighbour_increments = [(1,1), (1,-1), (-1,1), (-1,-1), (2,0), (-2,0)]
    to_flip = list()
    for coords in isWhiteUp:
        count = getNeighbourCount(coords, isWhiteUp)
        if isWhiteUp[coords] and count == 2:
            to_flip.append(coords)
        elif not isWhiteUp[coords] and (count == 0 or count > 2):
            to_flip.append(coords)
    for coords in to_flip:
        isWhiteUp[coords] = not isWhiteUp[coords]

def AddRequredTiles(isWhiteUp):
    neighbour_increments = [(1,1), (1,-1), (-1,1), (-1,-1), (2,0), (-2,0)]
    to_update = list()
    for coords in isWhiteUp:
        for neighbour_increment in neighbour_increments:
            new_coords = (coords[0] + neighbour_increment[0], coords[1] + neighbour_increment[1])
            if new_coords not in isWhiteUp:
                to_update.append(new_coords)
    for coords in to_update:
        if coords not in isWhiteUp:
            isWhiteUp[coords] = True

def getCooordinatesFromPath(path):
    i = 0
    x = 0
    y = 0
    while i < len(path):
        if path[i] == 'e':
            x = x+2
            i = i + 1
        elif path[i] == 'w':
            x = x-2
            i = i + 1
        elif path[i:i+2] == 'nw':
            x = x-1
            y = y+1
            i = i + 2
        elif path[i:i+2] == 'ne':
            x = x+1
            y = y+1
            i = i + 2
        elif path[i:i+2] == 'sw':
            x = x-1
            y = y-1
            i = i + 2
        elif path[i:i+2] == 'se':
            x = x+1
            y = y-1
            i = i + 2
    return x, y

def main():
    f = open("../cpp/day_24_data.txt")
    n_days = 100

    isWhiteUp = dict()
    for path in f:
        path = path.strip('\n')
        x, y = getCooordinatesFromPath(path)
        coord = (x,y)
        if coord in isWhiteUp:
            isWhiteUp[coord] = not isWhiteUp[coord]
        else:
            isWhiteUp[coord] = False
    for _ in range(0, n_days):
        AddRequredTiles(isWhiteUp)
        MakeArtLive(isWhiteUp)
    count = 0
    for key in isWhiteUp:
        if not isWhiteUp[key]:
            count = count + 1
    print(count)
    return count

if __name__ == "__main__":
    main()
