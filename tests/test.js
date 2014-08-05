"use strict";

var aggregate = require("mongodb-pipeline").aggregate,
	assert = require("assert"),
	sampleDocs1 = [{"v":3}, {"v":5}, {"v":2}, {"v":1}, {"v":4}];

describe("aggregate", function(){
	describe("error handling", function(){
		it("should require three arguments", function(done){
			assert.throws(aggregate, TypeError);
			assert.throws(aggregate, TypeError, []);
			assert.throws(aggregate, TypeError, [], []);
			assert.throws(aggregate, TypeError, [], [], done, []);
			aggregate([], [], done);
		});

		it("the first argument must be an array", function(done) {
			aggregate([], [], function(err) {
				if(err) return done(err);
				aggregate({}, [], function(err) {
					assert.equal(err.message, "BAD PIPELINE, MUST BE AN ARRAY!");
					return done();
				})
			});
		});

		it("the second argument must be an array", function(done) {
			aggregate([], [], function(err) {
				if(err) return done(err);
				aggregate([], {}, function(err) {
					assert.equal(err.message, "BAD DOCUMENT STREAM, MUST BE AN ARRAY!");
					return done();
				})
			});
		});

		it("the third argument must be a callback", function(done) {
			assert.throws(aggregate, TypeError, [], [], 4);
			aggregate([], [], function(err) {
				return done(err);
			});
		});
	});

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

	describe("$project", function(){
		it("should be able to rename a field", function(){
			var expected = [{"bar":3},{"bar":5},{"bar":2},{"bar":1},{"bar":4}],
				actual = aggregate([{$project:{"bar":"$v"}}], sampleDocs1);
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
		it("should be able to group on null", function(){
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
