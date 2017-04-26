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
const PORT = 8080;

app.use(express.static('public'))
app.use(bodyParser.json()); // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({ // to support URL-encoded bodies
    extended: true
}));

let FxCollection = function(app){
  var funcs = {};

  this.get = (id) => funcs[id];

  this.list = () => Object.keys(funcs);

  this.mount = function(fn){

    app[fn.method]('/' + fn.name, fn.handler);

  }

  this.add = function(fn){
    var handler = basic_adapter(fn.compiled)
    fn.handler = handler;
    funcs[fn.name] = fn;

    this.mount(fn);
  }
}

var fxs = new FxCollection(app);

app.get('/function', (req, res) => {
  res.status(200).send({'functions': fxs.list() });
})

app.post('/function', function(req, res) {

    var code = req.body.code;
    var fname = req.body.name;

    if (!_.isUndefined(code) && !_.isUndefined(fname)) {

        try {

            var fx = functor.compile(code);

            fxs.add({
                name: fname,
                method: 'get',
                source: code,
                compiled: fx
            });

            res.send({
                result: fname + MSG_COMPILE_STORE
            });

        } catch (e) {
            res.status(406).send({
                result: ERROR_MSG_2 + e
            });
        }

    } else {
        res.status(406).send({
            result: ERROR_MSG_1
        });
    }
})

app.get('/function/:id', function(req, res) {
  console.log('function->', req.params.id);
  var id = req.params.id;

  if(!_.isUndefined(id)){
    console.log('looking for: ', fxs.get(id));
    res.send(fxs.get(id));
  }else {
    res.status(404).send({});
  }
});

app.listen(PORT, function() {
    console.log('λ Functor  -> listening in ', PORT)
})
