let state = {};

let isLetter = c => c >= 'a' && c <= 'z';


let getVal = function(x) {
    if (isLetter(x[0])) {
        if (x in state) return state[x];
        throw new Error("oops");
    }
    return parseInt(x);
} 

let assign = x => getVal(x[0]);
let and = x => (getVal(x[0]) & getVal(x[2])) & 0xFFFF;
let or = x => (getVal(x[0]) | getVal(x[2])) & 0xFFFF;
let lshift = x => (getVal(x[0]) << getVal(x[2])) & 0xFFFF;
let rshift = x => (getVal(x[0]) >> getVal(x[2])) & 0xFFFF;
let not = x => (~getVal(x[1])) & 0xFFFF;

function selector(p) {
    if (p[1] === "->") return assign;
    else if (p[1] === "AND") return and;
    else if (p[1] === "OR") return or;
    else if (p[1] === "LSHIFT") return lshift;
    else if (p[1] === "RSHIFT") return rshift;
    else if (p[0] === "NOT") return not;
    throw new Error(`unknown selector ${p}`);
}

function getInstructions(input) {
    return input.map(i => i.split(' ')).map(i => ({ target:i[i.length-1], action:selector(i), params:i }));
}

function runInstructions(ins) {
    var i;

    while ((i = ins.shift()) !== undefined) {
        try {
            state[i.target] = i.action(i.params);
        }
        catch(ex) {
            if (ex.message === "oops") {
                ins.push(i);
            }
            else {
                throw ex;
            }
        }
    }
    return state["a"];
}

function part1(input) {
    state={};
    return runInstructions(getInstructions(input));
}

function part2(input) {
  state={};
  var ins = getInstructions(input);
  ins = ins.map(i => i.target === "b" ? { target:"b", action:assign, params:[part1Result.toString(),"->","b"]} : i)
  return runInstructions(ins);
}

const fs = require("fs");
const inputString = fs.readFileSync("input", "utf8");
const input = inputString.split('\n');

const part1Result = part1(input);
console.log(part1Result);

const part2Result = part2(input);
console.log(part2Result);