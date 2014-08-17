"use strict";

var _aggregate = require('bindings')('mdb-conduit.node');

function _aggregate_bare(docSrcs, documents, callback) {
	return _aggregate.aggregate(docSrcs, documents, callback);
}

function aggregate(docSrcs, documents, callback) {
	//TODO: get rid of the JSON.parse() here.  The implementation should be
	//returning us native V8 objects or bson.
	_aggregate_bare(docSrcs, documents, function fixupResults(err, results) {
		if(err) return callback(err);

		results = JSON.parse(results);
		return callback(null, results.result);
	});
}

module.exports = {
	aggregate: aggregate,
	_aggregate_bare: _aggregate_bare
};
