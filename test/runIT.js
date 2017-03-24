'use strict';

var serverz = require('../build/Debug/serverz');

function fib(n){
    if(n < 0) return 1;
      return fib(n-1) + fib(n-2);
};

console.log('serverz.run => ', serverz.run)
console.log('serverz.run (running fib(5)) => ', serverz.run(fib, [5])); 
