'use strict';

const _ = require('underscore');
const bodyParser = require('body-parser');
const express = require('express');

const ERROR_MSG_1 = "Missing parameters in the request call, this service accept {code:<string with javascript code>, name:<function name> }";
const ERROR_MSG_2 = "Error at compile time: ";
const MSG_COMPILE_STORE = ' has been compiled and is ready to use.';

const defaultRouter = function() {

    let router = new express.Router();

    router.use(cors());

    router.use(bodyParser.urlencoded({
        extended: true
    }));

    router.use(bodyParser.json());

    return router;
};

const functor = (req, res) => {

    if (!_.isUndefined(req.body.code) && !_.isUndefined(req.body.name)) {

        try {
            var fx = functor.compile(req.body.code);
            fmap[req.body.name] = fx;

            addRoute({
                routeName: req.body.name,
                method: 'get',
                handler: basic_adapter(fx)
            });

        } catch (e) {

            res.status(406).send({
                result: ERROR_MSG_2 + e
            });
        }

        console.log('compiled result->', fmap[req.body.name]);

        res.send({
            result: req.body.name + MSG_COMPILE_STORE
        });

    } else {
        res.status(406).send({
            result: ERROR_MSG_1
        });
    }
};

const handle = (router) => {
    router.post('/', functor);

    return router;
};

module.exports = {
    handle: handle(defaultRouter())
};
