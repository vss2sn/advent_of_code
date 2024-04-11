def read_initial_configuration(file_path):
  with open(file_path, 'r') as file:
      return list(map(int, file.readline().split()))

def redistribute(banks):
  max_value = max(banks)
  max_index = banks.index(max_value)
  banks[max_index] = 0
  index = max_index + 1
  while max_value > 0:
      if index == len(banks):
          index = 0
      banks[index] += 1
      max_value -= 1
      index += 1

def find_cycle_length(initial_banks):
  seen_configurations = set()
  cycle_length = 0
  while True:
      redistribute(initial_banks)
      cycle_length += 1
      configuration = tuple(initial_banks)
      if configuration in seen_configurations:
          return cycle_length
      seen_configurations.add(configuration)

# Example usage
initial_banks = read_initial_configuration('input')
cycle_length = find_cycle_length(initial_banks)
print(f"The number of redistribution cycles before a configuration is produced that has been seen before is: {cycle_length}")
