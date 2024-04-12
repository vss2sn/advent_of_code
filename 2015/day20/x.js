function solve(input, part) {
    const target = Number(input[0]);

    let presentsPerHouse = part === 1 ? 10 : 11;
    let houses = new Array(Math.trunc(target/presentsPerHouse) + 1).fill(0);

    for (let elf = 1; elf < houses.length; elf++)
        for (let house = elf, n = 0; house < houses.length && (part === 1 || n < 50); house+=elf, n++)
            houses[house] += elf * presentsPerHouse;
    for (let house = 1; house < houses.length; house++)
        if (houses[house] > target) { return house; }
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);