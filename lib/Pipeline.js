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
	//Note: these tests are NOT supposed to be exhaustive.  The MongoDB test cases are for that.
	//      The goal here is to hit most/all top-level functionality to make sure I've gotten
	//      the build right (no dynamic symbol failures.  This will become much less important
	//      once I'm directly building the mongo bits instead of linking with the libs.
	//TODO: add a gyp step to run the mongo pipeline test cases?  Would be cool if I could pull
	//      those in and compile them too.

	var numArray = [{"_id":5}, {"_id":2}, {"_id":10}, {"_id":7}, {"_id":10}],
		strings = [{"a":"a", "also_a":"a", "b":"b", "A":"A", "foO":"foO"}];

	//Document sources / Pipeline ops.
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
			//Group ops.
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

	console.log("boolean operators:");
	console.log(pipeline.aggregate([
		{"$limit":1},
		{"$project":{
			"_id":0,
			"false":{"$and":[0, null, undefined]},
			"true":{"$and":[10, {"bar":"$_id"}, new Date(), "hi there"]}, //Note: this counts on the first input doc having a non-falsey _id.
			"or_true_1":{"$or":[10, 0]},
			"or_true_2":{"$or":[0, new Date()]},
			"not_true":{"$not":{"$and":[0, null, undefined]}}
			}}
		],
		numArray
	));

	console.log("comparison operators:");
	console.log(pipeline.aggregate([
		{"$limit":1},
		{"$project":{
			"_id":0,
			"cmp_lt":{"$cmp":[0, 10]},
			"cmp_gt":{"$cmp":[1, -1]},
			"cmp_eq":{"$cmp":[42, 42]},
			"eq_num":{"$eq":[42, 42]},
			"eq_str":{"$eq":["$a", "$also_a"]},
			"gt_true":{"$gt":[42, 4]},
			"gt_false_1":{"$gt":[42, 54]},
			"gt_false_2":{"$gt":[42, 42]},
			"gte_eq_true":{"$gte":[42, 42]},
			"lt_true":{"$lt":[4, 42]},
			"lt_true_2":{"$lt":[42, 54]},
			"lt_false":{"$lt":[42, 42]},
			"lte_eq_true":{"$lte":[42, 42]},
			"ne_true":{"$eq":["$a", "$also_a"]},
			"ne_false":{"$eq":["$a", "$b"]}
			}}
		],
		strings
	));

	console.log("string operators:");
	console.log(pipeline.aggregate([
		{"$limit":1},
		{"$project":{
			"_id":0,
			"ab":{"$concat":["$a", "$b"]},
			"strcasecmp_eq_1":{"$strcasecmp":["$a", "$a"]},
			"strcasecmp_eq_2":{"$strcasecmp":["$a", "$A"]},
			"strcasecmp_lt":{"$strcasecmp":["$a", "$b"]},
			"strcasecmp_gt":{"$strcasecmp":["$b", "$a"]},
			"substr_o":{"$substr":["$foO", 1, 1]},
			"toLower_foo":{"$toLower":["$foO"]},
			"toUpper_FOO":{"$toUpper":["$foO"]}
			}}
		],
		strings
	));
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
