#!/usr/bin/python3

def main():
    n_rounds = 100
    n_lift = 3
    f = open("../input/day_23_input")
    line = f.readline().strip('\n')
    order_list = list()
    for char in line:
        order_list.append(int(char))
    order = dict()
    for i in range(0, len(order_list) - 1):
        order[order_list[i]] = order_list[i+1]
    order[order_list[-1]] = order_list[0]
    current_cup = order_list[0]
    low = min(order_list)
    high = max(order_list)
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
    ans = ""
    temp = 1
    for _ in range(0, len(order_list) - 1):
        temp = order[temp]
        ans = ans + str(temp)
    print(ans)
    return ans

if __name__ == "__main__":
    main()
