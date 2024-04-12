const fs = require('fs');
const path = require('path');

// 创建文件路径
const lst = Array.from({ length: 100 }, (_, i) => (i + 1).toString());

// 定义你想创建的文件夹路径
const dirPath = path.join(__dirname, ...lst);

// 使用mkdir方法创建文件夹，设置recursive为true以确保所有父文件夹都存在
fs.mkdir(dirPath, { recursive: true }, (err) => {
 if (err) {
    console.error('Error creating directory:', err);
 } else {
    console.log('Directory created successfully:', dirPath);
 }
});
