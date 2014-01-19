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

var unitTest = false;
if(unitTest) {
	var numArray = [{"_id":5}, {"_id":2}, {"_id":10}, {"_id":7}, {"_id":10}];

	console.log("$project:");
	console.log(pipeline.aggregate([{"$project":{"_id":0, "foo":"$_id"}}], numArray));

	console.log("$match:");
	console.log(pipeline.aggregate([{"$match":{"_id":{"$gte":5, "$lt":10}}}], numArray));

	console.log("$limit:");
	console.log(pipeline.aggregate([{"$limit":1}], [{"hello":"world!"}, {"second":"obj"}]));

	console.log("$skip:");
	console.log(pipeline.aggregate([{"$skip":3}], numArray));

	console.log("$unwind:");
	console.log(pipeline.aggregate([{"$unwind":"$items"}], [{"a":"foo", "z":"bar", "items":[1, 2]}]));

	console.log("$group:");
	console.log(pipeline.aggregate([
		{"$group":{
			"_id":null,
			uniqueSet:{"$addToSet":"$_id"},
			first:{"$first":"$_id"},
			last:{"$last":"$_id"},
			max:{"$max":"$_id"},
			min:{"$min":"$_id"},
			avg:{"$avg":"$_id"},
			set:{"$push":"$_id"},
			sum:{"$sum":"$_id"},
			count:{"$sum":1}
		}}],
		numArray
	));

	console.log("$sort:");
	console.log(pipeline.aggregate([{"$sort":{"_id":1}}], numArray));
}

var bigSubstractTest = false;
if(bigSubstractTest) {

	//lol, running with a million elements gives:
	// { error: 'running the pipeline failed.',
	//   message: 'Converting from JavaScript to BSON failed: Object size 34888895 exceeds limit of 16777216 bytes.' }
	//Good old 16 MiB limit paying me a visit.
	var arr=[]; for(var i=0; i < 100000; ++i) { arr.push({a:1000, v:Math.floor(Math.random()*1000)}); }

	subtractTest = [
		{"$project":{
			"r": {"$subtract":["$a", "$v"]}
		}},
		/*{"$group":{
			"_id":null,
			count:{"$sum":1},
			avg:{"$avg":"$r"},
			min:{"$min":"$r"},
			max:{"$max":"$r"}
		}}*/
	];

	//Not printing results to leave console I/O out of the picture.
	//console.log("started...");

	//Adding a $group (even though it's doing all that extra work) shaved 300ms
	//off of the time to run, proving that converting the results to BSON is a performance bottleneck.
	//Hopefully js-bson will be MUCH faster.
	_aggregate_bare(subtractTest, arr);
	//console.log(aggregate(subtractTest, arr));
	//console.log("...finished!");
}

module.exports = {
	aggregate: aggregate
};
