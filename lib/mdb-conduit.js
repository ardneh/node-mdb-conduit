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

var mdb_conduit = require('bindings')('mdb-conduit.node'),
		argv = ["mdb-conduit"],
		conduit_main = mdb_conduit._conduit_main;

//Default to json encoding for the pipeline and data.  The user can override
//this with --format.
Array.prototype.push.apply(argv, ["--format", "json-json"]);
Array.prototype.push.apply(argv, process.argv.slice(2));

setImmediate(conduit_main, argv, process.env);
