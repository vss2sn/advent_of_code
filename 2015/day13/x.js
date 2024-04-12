let {permutations,pairwise,max} = require("../day09/utils");

function solve(input, part) {
    let rules = input
    .map(s => s.match(/(\w+) would (lose|gain) (\d+) happiness units by sitting next to (\w+)\./))
    .map(p => { return { A: p[1], B: p[4], Gain: parseInt(p[3]) * (p[2] === "lose" ? -1 : 1) }});
    let people = Array.from(new Set(rules.map(r => r.A)));
    let lookup = rules.reduce((a,r) => { a[`${r.A}-${r.B}`] = r.Gain; return a}, {});
    if (part == 2) {
        people.forEach(p => { lookup[`Mark-${p}`] = 0; lookup[`${p}-Mark`] = 0; });
        people.push("Mark");
    }

    let lookupHappiness = ([a,b]) => lookup[`${a}-${b}`] + lookup[`${b}-${a}`];
    var happiness = permutations(Array.from(people)
        .slice(1))
        .map(p => { p.unshift(people[0]); p.push(people[0]); return pairwise(p); })
        .map(p => p.reduce((acc,x) => { return acc + lookupHappiness(x); },0));
    return max(happiness)
}

function expected(part) {
    return part == 1 ? 664 : 640;
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);