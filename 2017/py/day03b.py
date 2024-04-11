def update_grid(grid, col, row):
  sum = 0
  for i in range(-1, 2):
      for j in range(-1, 2):
          if row + j < 0 or col + i < 0:
              continue
          sum += grid[row + j][col + i]
  grid[row][col] = sum

def print_grid(grid):
  for row in range(len(grid) - 1, -1, -1):
      for col in range(len(grid[0])):
          print(grid[row][col], end=' ')
      print()

def check_if_left_empty(grid, dir, col, row):
  new_dir = (dir + 1) % 4
  if new_dir == 0:
      return grid[row + 1][col] == 0
  elif new_dir == 1:
      return grid[row][col - 1] == 0
  elif new_dir == 2:
      return grid[row - 1][col] == 0
  elif new_dir == 3:
      return grid[row][col + 1] == 0
  return False

def main():
  input_file = "input"
  with open(input_file, 'r') as file:
      number = int(file.readline().strip())

  if number == 1:
      print(2)
      return

  box_side = 1
  while number >= (box_side * box_side) * 2:
      box_side += 2

  distance = 0
  box_side_2 = box_side * box_side
  grid = [[0 for _ in range(box_side + 2)] for _ in range(box_side + 2)]

  col = box_side // 2 + 1
  row = box_side // 2 + 1
  grid[row][col] = 1
  col += 1
  grid[row][col] = 1
  dir = 3

  while number >= grid[row][col]:
      while not check_if_left_empty(grid, dir, col, row):
          if dir == 0:
              row += 1
          elif dir == 1:
              col -= 1
          elif dir == 2:
              row -= 1
          elif dir == 3:
              col += 1
          update_grid(grid, col, row)
          if grid[row][col] > number:
              print(grid[row][col])
              return
      dir = (dir + 1) % 4

if __name__ == "__main__":
  main()
