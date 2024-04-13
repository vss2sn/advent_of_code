import datetime

begin_time = datetime.datetime.now()
registers = {'a':0, 'b':0, 'c':0, 'd':0}
code = []
with open('./inputs/12.txt') as f:
    while line := f.readline().rstrip():
        code.append(line.split(' ', maxsplit=1))

pc = 0
while pc < len(code):
    cmd, args = code[pc]
    # print(cmd, args)

    if cmd == 'inc':
        registers[args] += 1
    elif cmd == 'dec':
        registers[args] -= 1
    elif cmd == 'jnz':
        x, y = args.split(' ')
        pc += int(y) if (x.isnumeric() and int(x)) or (x in registers and registers[x]) else 1
        continue
    elif cmd == 'cpy':
        x, y = args.split(' ')
        if x.isnumeric():
            registers[y] = int(x)
        else:
            registers[y] = registers[x]

    pc += 1

print(registers['a'])
print(datetime.datetime.now() - begin_time)