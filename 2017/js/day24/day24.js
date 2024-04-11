const fs = require('fs');
const { solve } = require('./solver24'); // 确保路径正确


// Function to read the input from a file
function readInputFromFile(filePath) {
    return fs.readFileSync(filePath, 'utf8').split('\n');
}

// Example usage
const inputFilePath = 'input'; // Replace with your file path
const input = readInputFromFile(inputFilePath);
const part1 = 1; // Set part to 1 for part 1
const part2 = 2; // Set part to 2 for part 2

const result1 = solve(input, part1);
console.log(`The result for part 1 is: ${result1}`);

const result2 = solve(input, part2);
console.log(`The result for part 2 is: ${result2}`);