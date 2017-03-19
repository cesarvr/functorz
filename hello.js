'use strict';
//var addon = require('./build/Release/serverz');
var addon = require('./build/Debug/serverz');
var fs = require('fs');
var os =  require('os');

//console.log(addon.compile(fs.readFileSync('./scripts/cnsl.js').toString('utf-8')));
let func = addon.compile(fs.readFileSync('./scripts/func.js').toString('utf-8'));
//let fib  = addon.compile(fs.readFileSync('./scripts/fibunacci.js').toString('utf-8')).result;


//console.log('fibunacci ->', fib(10))
console.log('func.js   ->', func)

