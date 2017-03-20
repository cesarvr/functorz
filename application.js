'use strict'; 

var express = require('express')
var functor = require('./build/Debug/serverz');

var app = express()

app.use(express.static('public'))

var fmap = {}; 

app.post('/function', function (req, res) {

  console.log(req.param); 
})

app.get('/function/:id', function (req, res) {

})

app.listen(8080, function () {
    console.log('Example app listening on port 8080!')
})

