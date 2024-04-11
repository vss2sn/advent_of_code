const fs = require('fs');
const { solve } = require('./solver23b'); // 确保路径正确


// Function to read the input from a file
function readInputFromFile(filePath) {
    return fs.readFileSync(filePath, 'utf8').split('\n');
}

// Example usage
const inputFilePath = 'input'; // Replace with your file path
const input = readInputFromFile(inputFilePath);
const part = 2; // Set part to 2 for part 2

const result = solve(input, part);
console.log(`The result for part 2 is: ${result}`);