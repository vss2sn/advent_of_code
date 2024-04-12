function solve(input, part) {
  var solver = part === 1 ? part1 : part2;
  return solver(input[0]);
}

function expected(part) {
  return part == 1 ? "vzbxxyzz" : "vzcaabcc";
}

const valid = "abcdefghjkmnpqrstuvwxyz";
const lookup = {}
for(let n = 1; n < valid.length; n++) {
  lookup[valid[n-1]] = valid[n];
}
let run = "";
for(let n = 0; n < valid.length - 2; n++) {
  run += valid.slice(n,n+3) + "|";
}
run = run.slice(0,-1);

function getNext(curr) {
  let out = ""
  let carry = true;
  for(var n = curr.length-1; n >=0; n--) {
      if(carry && curr[n] === 'z') {
          out = 'a' + out;
          carry = true;
      }
      else if (carry) {
          // String.fromCharCode(curr.charCodeAt(n) + 1)
          out = lookup[curr[n]] + out;
          carry = false;
      }
      else {
          out = curr[n] + out;
      }
  }
  if (carry) out = "z" + out;
  return out;
}

function isValid(pwd) {
  return pwd.match(/(\w)\1.*(\w)\2/g) && pwd.match(run);
}

function getNextValid(curr) {
  do{
      curr = getNext(curr);
  } while(!isValid(curr))
  return curr;
}

function part1(input) {
  return getNextValid(input);
}

function part2(input) {
  return getNextValid(getNextValid(input));
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);