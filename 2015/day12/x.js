function solve(input, part) {
  var solver = part === 1 ? part1 : part2;
  return solver(input);
}

function expected(part) {
  return part == 1 ? 111754 : 65402;
}

function isRed(obj) {
  if (obj.constructor == Array) return false;
  for(let j in obj) {
      if (obj[j] === "red") return true;
  }
  return false;
}

function sum(obj,avoid) {
  let total = 0;
  if (typeof(obj) === "object") {
      if (!avoid(obj)) {
          for (let j in obj) {
              total += sum(obj[j],avoid);
          }
      }
  }
  else if (typeof (obj) === "number") {
      total += obj;
  }
  return total;
}

function part1(input) {
  let j = JSON.parse(input.join("\n"))
  return sum(j,(() => false));
}

function part2(input) {
  let j = JSON.parse(input.join("\n"))
  return sum(j,isRed);
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);