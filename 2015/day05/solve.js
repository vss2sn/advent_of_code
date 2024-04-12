const fs = require("fs")
const input = fs.readFileSync("input", "utf8")
const hasThreeVowels = /[aeiou].*[aeiou].*[aeiou]/g
const hasDoubleLetter = /(.)\1/g
const containsNaughtyString = /ab|cd|pq|xy/g
const countNice = (input, isNice) => input.split("\n").filter(isNice).length
console.log(countNice(input, s => s.match(hasThreeVowels) && s.match(hasDoubleLetter) && !s.match(containsNaughtyString)));

function part2(input) {
    const containsNonOverlappingPair = /(\w{2}).*\1+/;
    const containsDuplicateSeparatedByOne = /(\w).\1/;
    return countNice(input, s => s.match(containsNonOverlappingPair) && s.match(containsDuplicateSeparatedByOne));
}

console.log(part2(input))