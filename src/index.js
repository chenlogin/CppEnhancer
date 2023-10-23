const titlebar = require('../build/Release/electron-windows-plugin.node');
const { db } = require('../db/database.js');

console.log('操作系统', process.arch)
console.log('C++ addon', titlebar.hello());
db();

module.exports = titlebar;
