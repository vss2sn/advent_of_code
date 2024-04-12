def look_and_say(sequence, times):
  for _ in range(times):
      new_sequence = ""
      i = 0
      while i < len(sequence):
          count = 1
          while i + 1 < len(sequence) and sequence[i] == sequence[i+1]:
              i += 1
              count += 1
          new_sequence += str(count) + sequence[i]
          i += 1
      sequence = new_sequence
  return sequence

initial_sequence = "3113322113"
result = look_and_say(initial_sequence, 50)
print(len(result)) # 输出结果
