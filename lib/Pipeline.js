
var pipeline = require('bindings')('pipeline.node');

function _aggregate_bare(docSrcs, documents) {
	return pipeline.aggregate(docSrcs, documents);
}

//TODO: this needs to take a callback and pass it into pipeline.aggregate()
//      so we can get rid of the funky error "handling" here.
function aggregate(docSrcs, documents) {
	//TODO: get rid of the JSON.parse() here.  The implementation should be
	//returning us native V8 objects or bson.
	var results = JSON.parse(_aggregate_bare(docSrcs, documents));
	if(results.error) {
		throw new Error(results.error);
	}

	return results.result;
}

var unitTest = false;
if(unitTest) {
	//Note: these tests are NOT supposed to be exhaustive.  The MongoDB test cases are for that.
	//      The goal here is to hit most/all top-level functionality to make sure I've gotten
	//      the build right (no dynamic symbol failures.  This will become much less important
	//      once I'm directly building the mongo bits instead of linking with the libs.
	//I got the list from http://docs.mongodb.org/manual/reference/operator/aggregation/
	//
	//TODO: add a gyp step to run the mongo pipeline test cases?  Would be cool if I could pull
	//      those in and compile them too.

	var numArray = [{"_id":5}, {"_id":2}, {"_id":10}, {"_id":7}, {"_id":10}],
		strings = [{"a":"a", "also_a":"a", "b":"b", "A":"A", "foO":"foO"}],
		dates = [{"testingTime":new Date('2014-01-19T14:45:30.539Z')}],
		nullsAndUndefinedsOhMy = [{"a_null":null, "an_undefined":undefined, "not_null":"nn", "someValue":42}];

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

	console.log("arithmatic operators:");
	console.log(pipeline.aggregate([
		{"$project":{
			"_id":0,
			"add_2":{"$add":[-1, 3]},
			"divide_2":{"$divide":[4, 2]},
			"mod_9":{"$mod":[9, 10]},
			"mulitply_54":{"$multiply":[6, 9]},
			"subtract_0":{"$subtract":[9, 9]},
			}}
		],
		[{"fun":true}]	
	));

	console.log("string operators:");
	console.log(pipeline.aggregate([
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

	console.log("date operators:");
	console.log(pipeline.aggregate([
		{"$project":{
			"_id":0,
			"dayOfYear_19":{"$dayOfYear":["$testingTime"]},
			"dayOfMonth_19":{"$dayOfMonth":["$testingTime"]},
			"dayOfWeek_1":{"$dayOfWeek":["$testingTime"]},
			"year_2014":{"$year":["$testingTime"]},
			"month_1":{"$month":["$testingTime"]},
			"week_3":{"$week":["$testingTime"]},
			"hour_14":{"$hour":["$testingTime"]},
			"minute_45":{"$minute":["$testingTime"]},
			"second_30":{"$second":["$testingTime"]},
			"millisecond_539":{"$millisecond":["$testingTime"]},
			}}
		],
		dates
	));

	console.log("conditional expressions:");
	console.log(pipeline.aggregate([
		{"$project":{
			"_id":0,
			"cond_54":{"$cond":[{"$eq":["$someValue", 42]}, {"$multiply":[6, 9]}, "Maybe the Universe makes sense after all!"]},
			"cond_2":{"$cond":["$an_undefined", 1, 2]},
			"ifNull_42":{"$ifNull":["$a_null", "$someValue"]},
			"ifNull_nn":{"$ifNull":["$not_null", "$someValue"]},
			}}
		],
		nullsAndUndefinedsOhMy
	));
}

var bigSubstractTest = false;
if(bigSubstractTest) {

	//lol, running with a million elements gives:
	// { error: 'running the pipeline failed.',
	//   message: 'Converting from JavaScript to BSON failed: Object size 34888895 exceeds limit of 16777216 bytes.' }
	//Good old 16 MiB limit paying me a visit.
	var arr=[],
		numItems = 100000;
	for(var i=0; i < numItems; ++i) { arr.push({a:1000, v:Math.floor(Math.random()*1000)}); }

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
	console.log("starting aggregation...");
	var startTime = Date.now();

	//Adding a $group (even though it's doing all that extra work) shaved 300ms
	//off of the time to run, proving that converting the results to BSON is a performance bottleneck.
	//Hopefully js-bson will be MUCH faster.
	var results = _aggregate_bare(subtractTest, arr),
		stopTime = Date.now();
	console.log("...finished aggregation, starting JSON.parse()...");
	JSON.parse(results);

	var stopParseTime = Date.now();
	console.log("...Done!\n");

	console.log("Aggregation time for %d items: %d ms\nParse time: %d ms\n", numItems, stopTime-startTime, stopParseTime-stopTime);

	//console.log(aggregate(subtractTest, arr));
}

module.exports = {
	aggregate: aggregate
};
