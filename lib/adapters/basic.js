'use strict';

const adapter = (fn) => {
    var fx = fn.result;

    return (req, res) => {
        var data = req.body || req.query;
        var result = null;

        try {
            result = fx(data);
        } catch (e) {
            res.status(500).send(e);
        }

        res.status(200).send(result);
    };
};

module.exports = adapter;
