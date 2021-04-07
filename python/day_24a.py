#!/usr/bin/python3

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
    f = open("../input/day_24_input")
    isWhiteUp = dict()
    for path in f:
        path = path.strip('\n')
        x, y = getCooordinatesFromPath(path)
        coord = (x,y)
        if coord in isWhiteUp:
            isWhiteUp[coord] = not isWhiteUp[coord]
        else:
            isWhiteUp[coord] = False
    count = 0
    for key in isWhiteUp:
        if not isWhiteUp[key]:
            count = count + 1
    print(count)
    return count

if __name__ == "__main__":
    main()
