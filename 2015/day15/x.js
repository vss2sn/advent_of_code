function solve(input, part) {
  let ingredients = input.map(f => f.match(/-?\d+/g).map(n => Number(n)));
  let max = -1;
  for(let c of distribute4(100)) {
      let s = scoreCookie(ingredients, c);
      if (part == 2 && s.calories != 500) s.score= -1;
      max = Math.max(s.score,max);
  }
  return max;
}

function expected(part) {
  return part == 1 ? 18965440 : 15862900;
}

function* distribute4(max) {
  for (let a = 0; a <= max; a++)
  for (let b = 0; b <= max - a; b++)
  for (let c = 0; c <= max - a - b; c++)
  yield [ a, b, c, max - a - b - c ];
}

function scoreCookie(ingredients, amounts) {
  let total = [0,0,0,0,0]
  for(let n = 0; n < ingredients.length; n++) {
      ingredients[n].forEach((t,q) => total[q] += t * amounts[n]);
  }
  let score = (Math.max(0,total[0]) * Math.max(0,total[1]) * Math.max(0,total[2]) * Math.max(0,total[3]))
  return { score, calories:total[4]};
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);