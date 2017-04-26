'use strict'

var assert = require('chai').assert;
var funktor = require('../build/Debug/serverz');

describe('Array', function() {
    describe('#compile', function() {

        it('correct instantiation ', function() {
            assert.isDefined(funktor.compile, 'compile is not defined');
            assert.isDefined(funktor.run, 'run is not defined');
        });

        it('invalid compilation error handling', function() {
            //Error compiling: SyntaxError: Unexpected token (
            assert.throws(function() {
                funktor.compile('function(){ return 1 }')
            }, Error, "Error compiling: SyntaxError: Unexpected token \(");
        });

        it('testing non-functions compilation', function() {

            var fn = funktor.compile('var x = 4;')
            assert.isUndefined(fn, 'should return undefined, when no function is detected');
        });

        it('compiling successfully', function() {

            var fn = funktor.compile('(function(){ return 1 })')
        });

        it('executing a function successfully', function() {
            var add = (a,b)=>{
              console.log('printing this content ->', this.x);
              return a+b;
            };

            var value = funktor.run(add, [1,2], {x:'hellos'}, {x:'gellop'});

            console.log('value returned by execution ', value);
        });

    });
});
