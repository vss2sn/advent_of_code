import re
from collections import defaultdict

def extract_supported_programs(s):
    supported_programs = []
    delimiter = ", "
    start = 0
    end = s.find(delimiter)
    while end != -1:
        supported_programs.append(s[start:end])
        start = end + len(delimiter)
        end = s.find(delimiter, start)
    supported_programs.append(s[start:])
    return supported_programs

def create_weight_map(map, base_name, weight_map):
    weight, supported_names = map[base_name]
    if not supported_names:
        weight_map[base_name] = weight
        return False, 0
    temp = defaultdict(list)
    weight_map[base_name] = weight
    for supported_name in supported_names:
        if supported_name not in weight_map:
            result = create_weight_map(map, supported_name, weight_map)
            if result[0]:
                return result
        weight_map[base_name] += weight_map[supported_name]
        temp[weight_map[supported_name]].append(supported_name)
    if len(temp) > 1:
        ans = 0
        for weight, names in temp.items():
            if len(names) == 1:
                ans += -weight + map[names[0]][0]
            else:
                ans += weight
        return True, ans
    return False, 0

def main(input_file):
    with open(input_file, 'r') as file:
        lines = file.readlines()
    pattern = re.compile(r'([a-z]+) \(([0-9]+)\)([a-z\ \,\-\>]?+)')
    map = {}
    supported_names_set = set()
    for line in lines:
        match = pattern.search(line)
        base = match.group(1)
        weight = int(match.group(2))
        optional_supported_names = match.group(3)
        supported_names = extract_supported_programs(optional_supported_names[4:-1]) if optional_supported_names else []
        map[base] = (weight, supported_names)
        supported_names_set.update(supported_names)

    main_base_name = next(name for name, data in map.items() if name not in supported_names_set)

    weight_map = {}
    result = create_weight_map(map, main_base_name, weight_map)
    print(result[1])

if __name__ == "__main__":
    main("input")
