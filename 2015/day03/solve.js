const fs = require("fs");
const { scan } = require("../day09/utils")

const input = fs.readFileSync("input", "utf8")

const lookup = { '>':[1,0], '^':[0,1], '<':[-1,0], 'v':[0,-1] };

function moveSanta(pos,c) {
    let [x,y] = lookup[c];
    return {x:pos.x+x,y:pos.y+y};
}

function part2(input) {
    let santaPos = {x:0,y:0};
    let roboSantaPos = {x:0,y:0};
    let positions = new Set();
    positions.add(`${santaPos.x},${santaPos.y}`);
    for(let n = 0; n < input.length; n+=2) {
        santaPos = moveSanta(santaPos,input[n]);
        roboSantaPos = moveSanta(roboSantaPos,input[n+1]);
        positions.add(`${santaPos.x},${santaPos.y}`);
        positions.add(`${roboSantaPos.x},${roboSantaPos.y}`);
    }
    return positions.size;
}

function part1(input) {
    let v = [...scan(input, moveSanta, {x:0,y:0})]
               .map(({x,y}) => `${x},${y}`);
    return new Set(v).size;
}

console.log(part1(input))
console.log(part2(input))