import requests

def find_santa_floor(instructions):
    floor_level = 0
    for char in instructions:
        if char == '(':
            floor_level += 1
        elif char == ')':
            floor_level -= 1
    return floor_level

# 使用HTTP请求获取指令
def get_instructions_via_http(url):
    response = requests.get(url)
    if response.status_code == 200:
        return response.text.strip() # 获取并去除空格和换行符
    else:
        print(f"Failed to get instructions from {url}. Status code: {response.status_code}")
        return None

# 示例URL，你应该替换为实际的URL
url = "https://raw.githubusercontent.com/markheath/advent-of-code-js/master/2015/01/input.txt"

# 使用HTTP请求获取指令
instructions = get_instructions_via_http(url)

if instructions:
    # 计算Santa应该去哪一层
    floor_level = find_santa_floor(instructions)
    print(f"Santa should go to floor: {floor_level}")
