'use strict'

var assert = require('chai').assert;
var funktor = require('../build/Debug/serverz');

describe('Array', function() {
    describe('#compile', function() {

        it('correct instantiation ', function() {
            assert.isDefined(funktor.compile, 'compile is not defined');
            assert.isDefined(funktor.run, 'run is not defined');
        });

        it('compile ', function() {
            try {
                var fn = funktor.compile('function(){ return 1 }');
            } catch (e) {
                console.log("error->funktor.compile > ", e);
            }

            console.log('typeof fn', typeof fn);
            assert.isFunction(fn, 'function should compile successfully');
        });


    });
});
