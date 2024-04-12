const readline = require('readline');

function solve(input, part, targetRow, targetCol) {
    if (part === 1) {
        let code = 20151125;
        for (let d = 1; ; d++) {
            for (let c = 1; c < d+1; c++) {
                let row = d - c + 1, col = c;
                if (row === targetRow && col === targetCol)
                    return code;
                code = (code * 252533) % 33554393;
            }
        }
    }
    return 0;
}

// 创建一个 readline.Interface 实例
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// 获取用户输入的行和列
rl.question('Please enter the target line: ', (targetRow) => {
    rl.question('Please enter the target column: ', (targetCol) => {
        // 调用 solve 函数并传入用户输入的行和列
        let result = solve(null, 1, parseInt(targetRow), parseInt(targetCol));

        // 输出结果
        console.log(result);

        // 关闭 readline.Interface 实例
        rl.close();
    });
});
