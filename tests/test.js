"use strict";

var aggregate = require("mongodb-pipeline").aggregate,
	assert = require("assert"),
	sampleDocs1 = [{"v":3}, {"v":5}, {"v":2}, {"v":1}, {"v":4}];

describe("aggregate", function(){
	describe("$limit", function(){
		it("should be able to limit the actual set", function(){
			var actual = aggregate([{$limit:1}], sampleDocs1);
			assert.deepEqual(actual, [{"v":3}]);
		});
	});

	describe("$match", function(){
		it("should be able to do a simple match", function(){
			var expected = [{"v":3},{"v":5},{"v":4}],
				actual = aggregate([{$match:{"v":{"$gte":3, "$lt":6}}}], sampleDocs1);
			assert.deepEqual(actual, expected);
		});
	});

	describe("$sort", function(){
		it("should be able to sort a simple set of documents", function(){
			var expected = [{"v":5},{"v":4},{"v":3},{"v":2},{"v":1}],
				actual = aggregate([{$sort:{"v":-1}}], sampleDocs1);
			assert.deepEqual(actual, expected);
		});
	});

	describe("$group", function(){
		it("should be able to group everything together", function(){
			var expected = [{"_id":null,"sum":15,"count":5}],
				actual = aggregate([{
						$group:{
							_id:null,
							sum:{$sum:"$v"},
							count:{$sum:1}
						}}],
						sampleDocs1
					);
			assert.deepEqual(actual, expected);
		});
	});

});

describe("_aggregate_bare", function(){
});
