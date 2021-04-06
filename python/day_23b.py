#!/usr/bin/python3

def main():
    highest_element = 1000000
    n_rounds = 10000000
    n_lift = 3

    f = open("../cpp/day_23_data.txt")
    line = f.readline().strip('\n')
    order_list = list()
    for char in line:
        order_list.append(int(char))

    order = dict()
    for i in range(0, len(order_list) - 1):
        order[order_list[i]] = order_list[i+1]
    order[order_list[-1]] = len(order_list) + 1
    i = len(order_list) + 1
    while i <= highest_element:
        order[i] = i+1
        i = i + 1
    order[len(order)] = order_list[0]

    current_cup = order_list[0]
    low = min(order_list)
    high = highest_element
    for round in range(0, n_rounds):
        remove_first = order[current_cup]
        current_now_connected_to = current_cup
        lifting = list()
        for _ in range(0, n_lift + 1):
            current_now_connected_to = order[current_now_connected_to]
            if (len(lifting) != n_lift):
                lifting.append(current_now_connected_to)
        order[current_cup] = current_now_connected_to
        dest_cup = current_cup - 1
        if (dest_cup < low):
            dest_cup = high
        while dest_cup in lifting:
            dest_cup = dest_cup - 1
            if (dest_cup < low):
                dest_cup = high
        temp = order[dest_cup]
        order[dest_cup] = remove_first
        val = remove_first
        for _ in range(0, n_lift - 1):
            val = order[val]
        order[val] = temp
        current_cup = order[current_cup]
    print(order[1])
    print(order[order[1]])
    ans = order[1] * order[order[1]]
    print(ans)
    return ans

if __name__ == "__main__":
    main()
