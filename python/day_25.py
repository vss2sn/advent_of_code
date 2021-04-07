#!/usr/bin/python3

def FindLoopSize(public_key,subject_number):
    count = 0;
    value = 1;
    while True:
        count = count + 1;
        value = value * subject_number;
        value = value % 20201227;
        if value == public_key:
            break
    return count

def GenerateEncrytionKey(subject_number, loop_size):
    value = 1
    for i in range(0, loop_size):
        value = value * subject_number
        value = value % 20201227
    return value

def main():
  file = open("../input/day_25_input")
  line = file.readline()
  line = line.strip('\n')
  card_public_key = int(line)
  card_subject_number = 7
  card_loop_size = FindLoopSize(card_public_key, card_subject_number)

  line = file.readline()
  line = line.strip('\n')
  door_public_key = int(line)
  door_subject_number = 7
  door_loop_size = FindLoopSize(door_public_key, door_subject_number)

  encryption_key = GenerateEncrytionKey(card_public_key, door_loop_size)
  print(encryption_key)
  return encryption_key

if __name__ == "__main__":
    main()
