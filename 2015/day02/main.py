import os
import requests

def measure(input, fn):
    # 分割字符串并解析为整数
    dimensions = [[int(n) for n in s.split('x')] for s in input]

    # 对每个矩形的尺寸进行排序
    sorted_dimensions = [sorted(dim) for dim in dimensions]

    # 应用传入的函数
    processed_dimensions = [fn(dim) for dim in sorted_dimensions]

    # 计算总和
    total = sum(processed_dimensions)

    return total

# 假设我们有一个URL，指向一个包含矩形尺寸的文本文件
url = "https://raw.githubusercontent.com/markheath/advent-of-code-js/master/2015/02/input.txt"
file_path = input("请输入文件路径: ")

# 检查文件是否存在
if os.path.exists(file_path):
    # 如果文件存在，则读取文件内容
    with open(file_path, "r") as file:
        file_content = file.read().splitlines()
else:
    # 如果文件不存在，则从远程URL获取文件内容
    response = requests.get(url)
    if response.status_code == 200:
        file_content = response.text.splitlines()
    else:
        print(f"Failed to fetch the file. Status code: {response.status_code}")
        file_content = []

# 定义一个函数，用于处理每个矩形的尺寸
# 这里是一个示例函数，它返回矩形的面积
def calculate_area1(w):
    return 3 * w[0] * w[1] + 2 * w[0] * w[2] + 2 * w[1] * w[2]

def calculate_area2(w):
    return 2 * w[0] + 2 * w[1] + w[0] * w[1] * w[2]
  
# 调用measure函数，传入文件内容和处理函数
total_area1 = measure(file_content, calculate_area1)

print(f"Total area1: {total_area1}")

total_area2 = measure(file_content, calculate_area2)

print(f"Total area2: {total_area2}")