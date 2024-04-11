def sum_of_division_results_from_file(filename):
  total_sum = 0
  with open(filename, 'r') as file:
      for line in file:
          row = list(map(int, line.strip().split()))
          for i in range(len(row)):
              for j in range(i + 1, len(row)):
                  if row[i] % row[j] == 0 or row[j] % row[i] == 0:
                      result = row[i] // row[j] if row[i] % row[j] == 0 else row[j] // row[i]
                      total_sum += result
                      break
              else:
                  continue
              break
  return total_sum

# 调用函数，传入文件名
result = sum_of_division_results_from_file('input')
print(f"The sum of each row's result is: {result}")
