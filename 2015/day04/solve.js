var crypto = require('crypto');
var fs = require('fs');

function find(key, search) {
    for (let n = 0; n < 10000000; n++) {
        let inputString = `${key}${n}`;
        let hash = crypto.createHash('md5').update(inputString).digest('hex');
        if (hash.startsWith(search)) return n;
    }
}

function part1(input) {
    return find(input, "00000");
}

function part2(input) {
    return find(input, "000000");
}

// 假设你的输入文件名为 'input.txt'
fs.readFile('input', 'utf8', (err, data) => {
    if (err) {
        console.error("Error reading file:", err);
        return;
    }
    // 确保文件内容已经被读取
    let input = data.trim(); // 使用 trim() 来移除可能的换行符
    console.log(part1(input));
    console.log(part2(input));
});
