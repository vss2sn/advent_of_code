function* scan(source, folder, startState) {
    let state = startState;
    if (typeof(state) !== "undefined" ) {
        yield startState;
    }
    for (let n of source) {
        if (typeof(state) === "undefined" ) {
            state = n;
        }
        else {
            state = folder(state, n);
        }
        yield state;
    }
}

//https://stackoverflow.com/a/20871714/7532
function permutations(inputArr) {
    let results = [];

    function permute(arr, memo) {
      let cur;
      memo = memo || [];

      for (var i = 0; i < arr.length; i++) {
        cur = arr.splice(i, 1);
        if (arr.length === 0) {
          results.push(memo.concat(cur));
        }
        permute(arr.slice(), memo.concat(cur));
        arr.splice(i, 0, cur[0]);
      }

      return results;
    }
    return permute(inputArr);
}

function* flatMap(xs,f) {
    for (let x of xs) {
        if (f) yield *f(x); else yield *x;
    }
}

function pairwise(arr) {
    let out = [];
    for (let n = 1; n < arr.length; n++) {
        out.push([arr[n-1],arr[n]]);
    }
    return out;
}

function* range(start, count) {
    for (let n = 0; typeof(count) === 'undefined' || n < count; n++) {
        yield start++;
    }
}

function* batch(seq, size) {
    let b = []
    for(let el of seq) {
        b.push(el)
        if (b.length === size) {
            yield b;
            b = []
        }
    }
    if (b.length > 0)
        yield b
}

function sumBy(seq, selector) {
    let total = 0;
    for(let n of seq) {
        total += (typeof(selector) === 'undefined') ? n : selector(n);
    }
    return total;
}

const sum = sumBy; // alias

function findBy(seq, selector, test) {
    let foundVal;
    let foundElem;
    for(let n of seq) {
        let val = (typeof(selector) === 'undefined') ? n : selector(n);
        if (typeof(foundElem) === 'undefined' || test(val,foundVal)) {
            foundElem = n;
            foundVal = val;
        }
    }
    return { foundVal, foundElem };
}

function maxBy(seq, selector) {
    return findBy(seq,selector,(a,b) => a > b).foundElem;
}

function minBy(seq, selector) {
    return findBy(seq,selector,(a,b) => a < b).foundElem;
}

function max(seq, selector) {
    return findBy(seq,selector,(a,b) => a > b).foundVal;
}

function min(seq, selector) {
    return findBy(seq,selector,(a,b) => a < b).foundVal;
}

function shuffle(array) {
    for (let i = array.length - 1; i > 0; i--) {
        let j = Math.floor(Math.random() * (i + 1));
        [array[i], array[j]] = [array[j], array[i]];
    }
}

function bfs(isSolution, getChildren, start)  {
    let q = [start]
    function *search() {
        if (q.length > 0) {
            let s = q.shift()
            if (isSolution(s)) {
                yield s;
            }
            q.push(...getChildren(s))
            yield* search()
        }
    }
    return search();
}

// way to get all matches for a regex
function* matches(str, regex) {
    let match;
    while((match = regex.exec(str)) !== null) {
        yield match;
    }
    return matches;
}

function any(seq, predicate) {
    if (typeof(predicate) === 'undefined') 
        return !seq[Symbol.iterator]().next().done
    for(let el of seq) {
        if (predicate(el)) return true;
    }
    return false;
}

function first(seq, predicate) {
    if (typeof(predicate) === 'undefined') 
        return seq[Symbol.iterator]().next().value
    for(let el of seq) {
        if (predicate(el)) return el;
    }
}

function* where(seq, filter) {
    for (let el of seq) {
        if (filter(el)) yield el;
    }
}

function* map(seq, fn) {
    for (let el of seq) {
        yield fn(el);
    }
}

function reduce(seq, fn, acc) {
    for (let el of seq) {
        acc = fn(acc,el);
    }
    return acc;
}

function count(seq, predicate) {
    let n = 0;
    for(let el of seq) {
        if (!predicate || predicate(el)) n++
    }
    return n;
}

function *take(seq, count) {
    let n = 0;
    for(let el of seq) {
        if (++n > count) return;
        yield el;
    }
}

function *skip(seq, count) {
    let n = 0;
    for(let el of seq) {
        if (++n > count) yield el;
    }
}

function *zip(seq1,seq2) {
    let it1 = seq1[Symbol.iterator]()
    let it2 = seq2[Symbol.iterator]()
    for(;;) {
        let el1 = it1.next()
        if(el1.done) return;
        let el2 = it2.next()
        if(el2.done) return;
        yield [el1.value,el2.value]
    }
}

function firstRepeated(seq, repeats, selector) {
    let current;
    let currentEl;
    let count = 0;
    for(let el of seq) {
        let testVal = (selector ? selector(el) : el)
        if (testVal === current) {
            count++;
            if (count >= repeats) return [current,currentEl]
        }
        else {
            current = testVal
            currentEl = el
            count = 1;
        }
    }
    throw new Error("Repeated element not found")
}

function firstRepeatedValue(seq, repeats, selector) {
    return firstRepeated(seq, repeats, selector)[0]
}

function firstRepeatedElement(seq, repeats, selector) {
    return firstRepeated(seq, repeats, selector)[1]
}


function nonRepeated(seq, selector) {
    let map = new Map() 
    for (let el of seq) {
        let key = selector ? selector(el) : el;
        if (!map.has(key)) {
            map.set(key, el)
        }
        else {
            map.set(key, undefined) // don't delete to eliminate three at same pos
        }
    }
    return where(map.values(),n => n !== undefined)
}

function* unfold(start, fn) {
    yield start;
    for(;;) {
        start = fn(start)
        yield start
    }
}

module.exports = { 
    scan,pairwise,permutations,flatMap,
    range,sumBy,maxBy,minBy,bfs,
    min,max,shuffle,matches,take,zip,skip,
    batch,sum,any,first,where,map,count,reduce,
    firstRepeatedValue,nonRepeated,unfold,firstRepeatedElement
}