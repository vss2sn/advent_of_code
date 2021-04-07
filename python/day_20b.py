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
        self.image_str = list()
        self.edges = list()

    def createEdges(self):
        left = str()
        right = str()
        for line in self.image_str:
            left = left + line[0]
            right = right + line[len(line) - 1]
        self.edges.append(self.image_str[0])
        self.edges.append(right)
        self.edges.append(self.image_str[len(self.image_str) - 1])
        self.edges.append(left)

    def getRightEdge(self):
        return self.edges[1]

    def getBottomEdge(self):
        return self.edges[2]

    def Rotate(self):
        self.edges.insert(0, self.edges[3])
        self.edges.pop()
        self.edges[0] = self.edges[0][::-1]
        self.edges[2] = self.edges[2][::-1]
        self.rotation = self.rotation + 1
        self.rotation = self.rotation % 4

    def Flip(self):
        temp = copy.deepcopy(self.edges[0])
        self.edges[0] = copy.deepcopy(self.edges[1])
        self.edges[1] = copy.deepcopy(temp)
        temp = copy.deepcopy(self.edges[3])
        self.edges[3] = copy.deepcopy(self.edges[2])
        self.edges[2] = copy.deepcopy(temp)

        for i in range(0, len(self.edges)):
            self.edges[i] = self.edges[i][::-1]
        self.flipped = not self.flipped

    def applyRotationsAndFlipToImage(self):
        if self.flipped:
            new_image = copy.deepcopy(self.image)
            for i in range(0, len(self.image)):
                for j in range(0, len(self.image[i])):
                    new_image[len(self.image[i]) - j - 1][len(self.image) - i - 1] = self.image[i][j];
            self.image = copy.deepcopy(new_image)
        for r in range(0, self.rotation):
            new_image = copy.deepcopy(self.image)
            for i in range(0, len(self.image)):
                for j in range(0, len(self.image[i])):
                    new_image[j][len(self.image) - i - 1] = self.image[i][j];
            self.image = copy.deepcopy(new_image)

def RotateImage(image):
    new_image = copy.deepcopy(image)
    for i in range(0, len(image)):
        for j in range(0, len(image[i])):
            new_image[j][len(image) - i - 1] = image[i][j];
    return new_image

def FlipImage(image):
    new_image = copy.deepcopy(image)
    for i in range(0, len(image)):
        for j in range(0, len(image[i])):
            new_image[len(image[i]) - j - 1][len(image) - i - 1] = image[i][j];
    return new_image


def OrientTileToMatchOnSide(edge, cur_tile, edge_dir):
    for i in range(0, 4):
        if edge == cur_tile.edges[edge_dir]:
            return
        else:
            cur_tile.Rotate()
    cur_tile.Flip()
    for i in range(0, 4):
        if edge == cur_tile.edges[edge_dir]:
            return
        else:
            cur_tile.Rotate()

def CheckForPattern(complete_image, pattern):
    max_row_size = 0
    relevant_points = list()
    for i in range(0, len(pattern)):
      relevant_points.append([])
      for j in range(0, len(pattern[i])):
          max_row_size = max(max_row_size, len(pattern[i]))
          if pattern[i][j] == '#':
              relevant_points[i].append(j)
    count = 0;
    for i in range(0, len(complete_image) - len(relevant_points)):
        for j in range(0, len(complete_image[i]) - max_row_size):
            match = True
            for pi in range(0, len(relevant_points)):
                for pj in range(0, len(relevant_points[pi])):
                    if complete_image[i+pi][j+relevant_points[pi][pj]] != '#':
                        match = False
                        break
                if not match:
                    break
            if match:
                count = count + 1
    return count

def backtrack(puzzle_size, count, grid, edge_map, tiles):
    if count == len(tiles):
        return True

    row = math.floor(count / puzzle_size)
    col = int(count % puzzle_size)
    if col == 0:
        bottom_edge = tiles[grid[row - 1][col]].getBottomEdge()
        ids = edge_map[bottom_edge]
        for id in ids:
            if id == grid[row - 1][0] or tiles[id].used:
                continue;
            cur_tile = tiles[id]
            cur_tile.used = True
            grid[row][col] = cur_tile.id
            OrientTileToMatchOnSide(bottom_edge, cur_tile, 0)
            if backtrack(puzzle_size, count + 1, grid, edge_map, tiles):
                return True
            else:
                cur_tile.used = False
                grid[row][col] = -1;
    elif row == 0:
        right_edge = tiles[grid[row][col - 1]].getRightEdge()
        ids = edge_map[right_edge]
        for id in ids:
            if id == grid[row][col - 1] or tiles[id].used:
                continue;
            cur_tile = tiles[id]
            cur_tile.used = True
            grid[row][col] = cur_tile.id
            OrientTileToMatchOnSide(right_edge, cur_tile, 3)
            if backtrack(puzzle_size, count + 1, grid, edge_map, tiles):
                return True
            else:
                cur_tile.used = False
                grid[row][col] = -1;
    elif row != 0:
        bottom_edge = tiles[grid[row - 1][col]].getBottomEdge()
        ids = edge_map[bottom_edge]
        for id in ids:
            if id == grid[row - 1][col] or tiles[id].used:
                continue;
            cur_tile = tiles[id]
            cur_tile.used = True
            grid[row][col] = cur_tile.id
            OrientTileToMatchOnSide(bottom_edge, cur_tile, 0)
            right_edge = tiles[grid[row][col - 1]].getRightEdge()
            if cur_tile.edges[3] == right_edge and backtrack(puzzle_size, count + 1, grid, edge_map, tiles):
                return True
            else:
                cur_tile.used = False
                grid[row][col] = -1;
    return False

def main():
    f = open("../input/day_20_input")
    new_tile = Tile()
    tiles = dict()
    edge_map = dict()
    for line in f:
        line = line.strip('\n')
        if line == "":
            new_tile.createEdges()
            tiles[new_tile.id] = copy.deepcopy(new_tile)
            new_tile = Tile()
            continue
        elif line[0:4] == "Tile":
            new_tile.id = int(line[5:len(line) - 1])
        else:
            new_tile.image_str.append(line)
            new_tile.image.append([])
            for char in line:
                new_tile.image[len(new_tile.image) - 1].append(char)
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

    found = False
    count = 1
    puzzle_size = int(math.sqrt(len(tiles)))
    grid = list()
    for i in range(0, puzzle_size):
        grid.append([-1] * puzzle_size)
    for corner_id in corner_ids:
        grid[0][0] = corner_id
        tiles[corner_id].used = True
        for i in range(0, 4):
            if backtrack(puzzle_size, count, grid, edge_map, tiles):
                found = True
                break
            tiles[corner_id].Rotate()
        if found:
            break
        tiles[corner_id].Flip();
        for i in range(0, 4):
            if backtrack(puzzle_size, count, grid, edge_map, tiles):
                found = True
                break
            tiles[corner_id].Rotate()
        if found:
            break
        else:
            tiles[corner_id].used = False
    complete_image = list()
    for i in range(0, puzzle_size):
        for j in range(0, puzzle_size):
            tiles[grid[i][j]].applyRotationsAndFlipToImage()
            tile_img = tiles[grid[i][j]].image
            for k in range(0, len(tile_img) - 2):
                # if (j == 0):
                #     complete_image.append([])
                if i * (len(tile_img) - 2) + k >= len(complete_image):
                    complete_image.append([])
                complete_image[i * (len(tile_img) - 2) + k].extend(tile_img[k+1][1:len(tile_img[k]) - 1]);
    pattern_temp = ["                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "]
    pattern = list()
    for i in range(0, len(pattern_temp)):
        pattern.append([])
        for char in pattern_temp[i]:
            pattern[i].append(char)

    monster_size = 0
    for row in pattern:
        for ele in row:
            if ele == '#':
                monster_size = monster_size + 1

    n_monsters = 0
    for i in range(0, 2):
        for j in range(0, 4):
            n_monsters = CheckForPattern(complete_image, pattern)
            if n_monsters > 0:
                break
            complete_image = RotateImage(complete_image)
        if n_monsters > 0:
            break
        complete_image = FlipImage(complete_image)

    roughness = 0
    for i in range(0, len(complete_image)):
        for j in range(0, len(complete_image[i])):
            if complete_image[i][j] == '#':
                roughness = roughness + 1
    roughness = roughness - (n_monsters * monster_size)
    print(roughness)
    return roughness

if __name__ == "__main__":
    main()
