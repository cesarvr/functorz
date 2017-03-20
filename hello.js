'use strict';
//var addon = require('./build/Release/serverz');
var addon = require('./build/Debug/serverz');
var fs = require('fs');
var os = require('os');
var http = require('http');

//console.log(addon.compile(fs.readFileSync('./scripts/cnsl.js').toString('utf-8')));
let func = addon.compile(fs.readFileSync('./scripts/func.js').toString('utf-8'));
let fib = addon.compile(fs.readFileSync('./scripts/fibunacci.js').toString('utf-8')).result;


var service = {
    os: () => os,
    http: () => http
}

console.log('fibunacci ->', fib(10))
console.log('func.js   ->', func)
var inject = func.result.bind({os:os});

if (typeof inject === 'function') console.log(inject(5, service, os ));
