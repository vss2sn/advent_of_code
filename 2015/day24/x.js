let {sumBy} = require("./2015/day09/utils");

let bestSoFar;
function solve(input, part) {
    var presents = input.map(n => Number(n));
    return findBestQE(presents, part === 1 ?  3:4);
}

function findBestQE(presents, groups)
{
    let totalWeight = sumBy(presents);
    let weightPerSet =  totalWeight / groups;
    bestSoFar = 1 + presents.length / groups;
    let bestSet = [...distribute([], presents, Math.floor(weightPerSet))]
        .map(g => { return { count: g.length, qe: g.reduce((a, b) => a * b, 1) } })
        .sort((a,b) => (a.count === b.count) ? a.qe - b.qe : a.count - b.count )[0]
    return bestSet.qe;
}

function* distribute(used, pool, amount)
{
    if (used.length >= bestSoFar) return;

    let remaining = amount - sumBy(used);
    for (let n = 0; n < pool.length; n++) {
        let s = pool[n];
        if (pool[n] > remaining) continue;
        let x = [...used,s];
        if (s === remaining) {
            if (x.length < bestSoFar)
                bestSoFar = x.length;
            yield x;
        }
        else {
            var y = pool.slice(n+1);
            yield* distribute(x, y, amount);
        }
    }
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);