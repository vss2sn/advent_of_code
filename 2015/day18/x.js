let {range,sumBy} = require("./2015/day09/utils");

function solve(input, part) {
    let state = input.map(s => Array.from(s.trim()).map(c => c === '#' ? 1 : 0));
    const repetitions = 100;
    let getNextValue = (part === 1) ? getNextValue1 : getNextValue2;
    let getLight = (part === 1) ? getLight1 : getLight2;
    for (let a = 0; a < repetitions; a++) {
        let nextState = [...range(0, state.length)]
            .map(x => [...range(0, state[x].length)]
                        .map(y => getNextValue(getLight, state, x, y)));
        state = nextState;
    }
    return sumBy(state, r => sumBy(r));
}

let getLight1 = (state, x, y) => {
    if (x < 0 || x >= state.length) return 0;
    if (y < 0 || y >= state[x].length) return 0;
    return state[x][y];
};

let getLight2 = (state, x, y) => {
    if (x < 0 || x >= state.length) return 0;
    if (y < 0 || y >= state[x].length) return 0;
    if (x == 0 && y == 0) return 1;
    if (x == state.length - 1 && y == 0) return 1;
    if (x == 0 && y == state[x].length - 1) return 1;
    if (x == state.length - 1 && y == state[x].length - 1) return 1;
    return state[x][y];
};

let getNeighbourSum = (getLight, state, x, y) => getLight(state, x - 1, y - 1) + 
                                        getLight(state, x, y - 1) + 
                                        getLight(state, x + 1, y - 1) +
                                        getLight(state, x - 1, y) + 
                                        getLight(state, x + 1, y) +
                                        getLight(state, x - 1, y + 1) + 
                                        getLight(state, x, y + 1) + 
                                        getLight(state, x + 1, y + 1);


let getNextValue1 = (getLight, state, x, y) => {
    return getNeighbourSum(getLight, state, x, y) === 3 ? 1 :
    (getNeighbourSum(getLight, state, x, y) === 2 && getLight(state, x, y) === 1) ? 1 : 0;
};

let getNextValue2 = (getLight, state, x, y) => {
    if (x == 0 && y == 0) return 1;
    if (x == state.length - 1 && y == 0) return 1;
    if (x == 0 && y == state[x].length - 1) return 1;
    if (x == state.length - 1 && y == state[x].length - 1) return 1;

    return getNeighbourSum(getLight, state, x, y) === 3 ? 1 :
   (getNeighbourSum(getLight, state, x, y) === 2 && getLight(state, x, y) === 1) ? 1 : 0;
};

function expected(part) {
    return part == 1 ? 814 : 924;
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = solve(input, 1);
console.log(part1Result);

const part2Result = solve(input, 2);
console.log(part2Result);