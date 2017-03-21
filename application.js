'use strict';

var express = require('express')
var functor = require('./build/Debug/serverz');
var _ = require('underscore');
var bodyParser = require('body-parser')
var app = express()

const ERROR_MSG_1 = "Missing parameters in the request call, this service accept {code:<string with javascript code>, name:<function name> }";
const ERROR_MSG_2 = "Error at compile time: ";
const MSG_COMPILE_STORE = ' has been compiled and is ready to use.';

app.use(express.static('public'))
app.use(bodyParser.json()); // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({ // to support URL-encoded bodies
    extended: true
}));

var fmap = {};

app.post('/function', function(req, res) {

    console.log(req.body);
    if (!_.isUndefined(req.body.code) && !_.isUndefined(req.body.name)) {

        try {
            fmap[req.body.name] = functor.compile(req.body.code);
        } catch (e) {

            res.status(406).send({
                result: ERROR_MSG_2 + e
            });
        }

        console.log('compiled result->', fmap[req.body.name]);

        res.send({
            result: req.body.name + MSG_COMPILE_STORE
        });

    } else
        res.status(406).send({
            result: ERROR_MSG_1
        });

})


app.get('/function/:id', function(req, res) {

})

app.listen(8080, function() {
    console.log('Example app listening on port 8080!')
})
