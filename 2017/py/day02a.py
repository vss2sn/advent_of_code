import sys

def calculate_checksum(filename):
      checksum = 0
      with open(filename, 'r') as file:
          for line in file:
              # 使用split方法处理可能存在的制表符或其他分隔符
              numbers = line.strip().split()
              if not numbers:
                  continue # 跳过空行
              min_val = max_val = int(numbers[0])
              for number in numbers[1:]:
                  val = int(number)
                  max_val = max(val, max_val)
                  min_val = min(val, min_val)
              checksum += max_val - min_val
      return checksum

if __name__ == "__main__":
      input_file = sys.argv[1] if len(sys.argv) > 1 else "input"
      checksum = calculate_checksum(input_file)
      print(checksum)
