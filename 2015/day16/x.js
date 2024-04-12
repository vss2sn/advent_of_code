let { matches } = require('../day09/utils')

function solve(input, part) {
    let sues = input.map(s => [...matches(s,/(\w+): (\d+)/g)].reduce((acc,m) => {
        acc[m[1]] = Number(m[2]); return acc;} ,{} ));

    if (part === 1) {
        return sues.findIndex(s => every(s,f1)) + 1;
    }
    return sues.findIndex(s => every(s,f2)) + 1;
}

// equivalent of Array.every for an object
function every(obj, matcher) {
    for(var prop in obj) {
        if (!matcher(prop, obj[prop])) return false;
    }
    return true;
}



let clues = {
    children: 3, 
    cats: 7,
    samoyeds: 2,
    pomeranians: 3,
    akitas: 0,
    vizslas: 0,
    goldfish: 5,
    trees: 3,
    cars: 2,
    perfumes: 1
};

let f1 = (t,n) => clues[t] === n;
function f2(t,n) { 
    if (t === "cats" || t === "trees") return n > clues[t];
    if (t === "pomeranians" || t === "goldfish") return n < clues[t];
    return n === clues[t];
}

function expected(part) {
    return part == 1 ? 213 : 323;
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);