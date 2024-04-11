function solve(input, part) {
    const instructions = input.map(i => i.split(' '))

    if (part === 1) {
        const int = interpreter(instructions, 0)
        while(!int.execute()) {
            //
        }
        return int.mult();
    }
    // counting non-primes
    let h = 0
    for(let b = 106700; b <= 123700; b += 17) { 
        if (!isPrime(b)) h++;
    }
    return h;
}

function interpreter(instructions, a) {
    const registers = new Map();
    registers.set('a',a)
    let curPos = 0
    let multiplies = 0;
    const getVal = v => isNaN(v) ? (registers.get(v) || 0) : Number(v)
    const commands = {
        set : (x,y) => registers.set(x,getVal(y)),
        sub : (x,y) => registers.set(x,getVal(x) - getVal(y)),
        mul : (x,y) => { multiplies++; registers.set(x,getVal(x) * getVal(y)) },
        jnz : (x,y) => curPos += (getVal(x) != 0 ? getVal(y) : 1)
    }
    const execute = () => {
        let [ins,arg1,arg2] = instructions[curPos]
        commands[ins](arg1,arg2)
        if (ins != "jnz") curPos++
        return (curPos < 0 || curPos >= instructions.length);
    }
    function showState() {
        console.log(curPos,instructions[curPos],registers)
    }
    return {execute,showState,mult:() => multiplies}
}

const isPrime = num => {
    for(let i = 2, s = Math.sqrt(num); i <= s; i++)
        if(num % i === 0) return false; 
    return num !== 1;
}

const expected = part => part === 1 ? 4225 : 905

module.exports = { solve, expected }