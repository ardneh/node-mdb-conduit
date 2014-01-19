//TODO: handle debug versions of node!
var pipeline = require(__dirname + '/../build/Release/pipeline.node');
//var pipeline = require(__dirname + '/../build/Debug/pipeline.node');

function _aggregate_bare(docSrcs, documents) {
	return pipeline.aggregate(docSrcs, documents);
}

function aggregate(docSrcs, documents) {
	//TODO: get rid of the JSON.parse() here.  The implementation should be doing that.
	return JSON.parse(_aggregate_bare(docSrcs, documents));
}

function testBSONConversion(data) {
	console.log(data, "=?", pipeline.aggregate(data, []));
}
//testBSONConversion([{'v':132},{'v':377},{'v':358},{'v':108},{'v':28},{'v':741},{'v':68}]);
//testBSONConversion([42]);
//testBSONConversion([1,2,3,{'a':'b'}]);

//The future:
console.log(pipeline.aggregate([{"$limit":1}], [{"hello":"world!"}, {"second":"obj"}]));

//Is now.
//...Well, kind of.  Running with a million elements gives: { error: 'running the pipeline failed.', message: 'Converting from JavaScript to BSON failed: Object size 34888895 exceeds limit of 16777216 bytes.' }
var arr=[]; for(var i=0; i < 100000; ++i) { arr.push({a:1000, v:Math.floor(Math.random()*1000)}); }

subtractTest = [
	{"$project":{
		"r": {"$subtract":["$a", "$v"]}
	}}
];

//Not printing results to leave console I/O out of the picture.
console.log("started...");
_aggregate_bare(subtractTest, arr);
console.log("...finished!");

module.exports = {
	aggregate: aggregate
};
