def realloc(banks):
  history = [list(banks)]
  while True:
      banks = redistribute(banks)
      if banks in history:
          return len(history) - history.index(banks)
      else:
          history.append(list(banks))

def redistribute(banks):
  index, blocks = max(enumerate(banks), key=lambda x: x[1])
  banks[index] = 0
  while blocks:
      index = (index + 1) % len(banks)
      banks[index] += 1
      blocks -= 1
  return banks

def main():
  with open('input', 'r') as file:
      input_data = file.read().strip()
  banks = list(map(int, input_data.split()))
  result = realloc(banks)
  print('Cycles in the infinite loop:', result)

if __name__ == "__main__":
  main()
