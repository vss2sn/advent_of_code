#!/usr/bin/python3

import math
import copy
import builtins


class Tile:
    def __init__(self):
        self.used = False
        self.flipped = False
        self.id = -1
        self.rotation = 0
        self.neighbour_ids = set()
        self.image = list()
        self.edges = list()

    def createEdges(self):
        left = str()
        right = str()
        for line in self.image:
            left = left + line[0]
            right = right + line[len(line) - 1]
        self.edges.append(self.image[0])
        self.edges.append(right)
        self.edges.append(self.image[len(self.image) - 1])
        self.edges.append(left)

    def Rotate(self):
        self.edges.insert(self.edges[3])
        self.edges.pop()
        self.edges[0][::-1]
        self.edges[2][::-1]
        rotation = rotation + 1
        rotation = rotation % 4

    def Flip(self):
        self.edges[0], self.edges[1] = self.edges[1], self.edges[0]
        self.edges[3], self.edges[2] = self.edges[2], self.edges[3]
        for edge in self.edges:
            edge[::-1]
        flipped = not flipped


def main():
    f = open("../input/day_20_input")
    new_tile = Tile()
    tiles = dict()
    edge_map = dict()
    for line in f:
        line = line.strip("\n")
        if line == "":
            new_tile.createEdges()
            tiles[new_tile.id] = copy.deepcopy(new_tile)
            new_tile = Tile()
            continue
        elif line[0:4] == "Tile":
            new_tile.id = int(line[5 : len(line) - 1])
        else:
            new_tile.image.append(line)
    if new_tile.id != -1:
        new_tile.createEdges()
        tiles[new_tile.id] = new_tile

    for tile_id in tiles:
        tile = tiles[tile_id]
        for edge in tile.edges:
            if edge not in edge_map:
                edge_map[edge] = [tile_id]
                r_edge = edge[::-1]
                edge_map[r_edge] = [tile_id]
            else:
                if tile_id not in edge_map[edge]:
                    edge_map[edge].append(tile_id)
                    r_edge = edge[::-1]
                    edge_map[r_edge].append(tile_id)

    for edge in edge_map:
        ids = edge_map[edge]
        for i in range(0, len(ids)):
            for j in range(i + 1, len(ids)):
                if j not in tiles[ids[i]].neighbour_ids:
                    tiles[ids[i]].neighbour_ids.add(ids[j])
                if i not in tiles[ids[j]].neighbour_ids:
                    tiles[ids[j]].neighbour_ids.add(ids[i])

    corner_ids = list()
    for id in tiles:
        tile = tiles[id]
        if len(tile.neighbour_ids) == 2:
            corner_ids.append(id)

    product = 1
    for corner_id in corner_ids:
        product *= corner_id
    print(product)
    return product


if __name__ == "__main__":
    main()
