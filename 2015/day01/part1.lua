print("请输入你的文件路径(默认为input):")
local path = io.read()
if path == "" then
  path = "input"
end
local file = io.open(path, "r")
local instructions = file:read("*all")
file:close()

function find_floor_level(instructions)
  local floor_level = 0
  for i = 1, #instructions do
    local char = instructions:sub(i,i)
    if char == "(" then
      floor_level = floor_level + 1
    elseif char == ")" then
      floor_level = floor_level - 1
    end
  end
  return floor_level
end

print("圣诞老人🎅应该到" .. find_floor_level(instructions) .. "层")
