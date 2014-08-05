"use strict";

var assert = require("assert"),
      _aggregate_bare = require("mongodb-pipeline")._aggregate_bare;

describe("aggregate performance", function(){
   xit("should be fast", function(done){
      //lol, running with a million elements gives:
      // { error: 'running the pipeline failed.',
      //   message: 'Converting from JavaScript to BSON failed: Object size 34888895 exceeds limit of 16777216 bytes.' }
      //Good old 16 MiB limit paying me a visit.
      var arr=[],
         numItems = 100000;
      for(var i=0; i < numItems; ++i) {
         arr.push({a:1000, v:Math.floor(Math.random()*1000)});
      }

      var subtractTest = [
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
      var results = _aggregate_bare(subtractTest, arr, function(err, results) {
         if(err) return done(err);

         var stopTime = Date.now();
         console.log("...finished aggregation, starting JSON.parse()...");

         //TODO: see if putting this in a closure slows it down as it appears to have from when this
         //was in Pipeline.js.
         JSON.parse(results);

         var stopParseTime = Date.now();
         console.log("...Done!\n");

         console.log("Aggregation time for %d items: %d ms\nParse time: %d ms\n", numItems, stopTime-startTime, stopParseTime-stopTime);

         return done();
      });

      //console.log(aggregate(subtractTest, arr));
   });
});
