/**
Provide mdb-aggregate, a MongoDB style aggregation pipeline, to NodeJS.

Copyright (C) 2014  Charles J. Ezell III

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

"use strict";

var _aggregate = require('bindings')('mdb-conduit.node');

function _aggregate_bare(docSrcs, documents, callback) {
	setImmediate(_aggregate.aggregate(docSrcs, documents, callback));
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
