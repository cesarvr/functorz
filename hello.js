var addon = require('./build/Release/serverz');
var fs = require('fs');

console.log(addon.hello()); // 'world'

//console.log(addon.compile(fs.readFileSync('./scripts/example_cnsl.js').toString('utf-8')));
console.log(addon.compile(fs.readFileSync('./scripts/fibunacci.js').toString('utf-8')));
console.log(addon.compile(fs.readFileSync('./scripts/func.js').toString('utf-8')));
console.log(fib(5))

