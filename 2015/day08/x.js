let [q,bq,bb] = ["\"","\\\"","\\\\"];

let unescape = s => s.slice(1,-1).replace(/\\"/g, q).replace(/\\\\/g, "?").replace(/\\x[0-9a-f]{2}/g,"?");
let escape = s => q + s.replace(/\\/g, bb).replace(/"/g, bq) + q;

let sumDiffLengths = (strings,f) => strings.map(f).reduce((acc,[a,b]) => acc + (a.length - b.length), 0);
let part1 = input => sumDiffLengths(input, f => [f, unescape(f)]);
let part2 = input => sumDiffLengths(input, f => [escape(f), f]);

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = part1(input);
console.log(part1Result);

const part2Result = part2(input);
console.log(part2Result);