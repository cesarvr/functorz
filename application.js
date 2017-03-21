'use strict';

var express = require('express')
var functor = require('./build/Debug/serverz');
var basic_adapter = require('./lib/adapters/basic');
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

var add_router = function(options) {
    app[options.method]('/' + options.routeName, options.handler);
    console.log('adding route ->', options.routeName, ' method->', options.method);
};

app.post('/function', function(req, res) {

    var code = req.body.code;
    var fname = req.body.name;

    if (!_.isUndefined(code) && !_.isUndefined(fname)) {

        try {

            var fx = functor.compile(code);

            fmap[fname] = {
                compiled: fx,
                source: code
            };

            add_router({
                routeName: fname,
                method: 'get',
                handler: basic_adapter(fx)
            });

        } catch (e) {
            res.status(406).send({
                result: ERROR_MSG_2 + e
            });
        }

        console.log('compiled result->', fmap[fname]);

        res.send({
            result: fname + MSG_COMPILE_STORE
        });

    } else
        res.status(406).send({
            result: ERROR_MSG_1
        });
})


app.get('/function', function(req, res) {

  Object.keys(fmap).map
})


app.get('/function/:id', function(req, res) {
  console.log('function->', req.params.id);
  var id = req.params:id; 

  console.log('looking for: ', fmap[id]);

})

app.listen(8080, function() {
    console.log('Example app listening on port 8080!')
})
