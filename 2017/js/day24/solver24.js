const { max} = require('./utils')

let bridges;
function solve(input,part) {
    let parts = input.map(i => i.split('/').map(n => Number(n)))
    bridges = bridges || [...build(0,[],parts,0)]
    if (part === 1) {
        return max(bridges,c => c.strength)
    }
    else {
        return bridges.sort((a,b) => b.used.length - a.used.length || b.strength - a.strength)[0].strength
    }
}

function *build(cur,used,available,strength) {
    for(let [a,b] of available) {
        if(a === cur || b === cur) {
            yield* build(a === cur ? b : a,[...used,[a,b]],available.filter(([x,y]) => !(x===a && y===b)),strength+a+b)
        }
    }
    yield {used,strength}
}

const expected = part => part === 1 ? 2006 : 1994;

module.exports = { solve, expected }