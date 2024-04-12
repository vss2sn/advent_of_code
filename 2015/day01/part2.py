import requests

def find_basement_position(instructions):
  floor = 0
  for position, char in enumerate(instructions, start=1):
    if char == "(":
      floor += 1
    elif char == ")":
      floor -= 1
    if floor == -1:
      return position
  return None
  
url =  "https://raw.githubusercontent.com/markheath/advent-of-code-js/master/2015/01/input.txt"

response = requests.get(url)

if response.status_code == 200:
  instructions = response.text
  position = find_basement_position(instructions)
  if position:
    print(f"The first basement position is {position}")
  else:
    print("No basement position found")
else:
  print(f"Error: {response.status_code}")