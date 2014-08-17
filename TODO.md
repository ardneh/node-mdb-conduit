* Use jshint and a C++ lint.
* Use thread pool instead of trying to make any part of the pipeline async,
  which is pretty much pointless because of $group and $sort).
* Use C++11 or, preferrably, 14.
* Try the https://github.com/mongodb/js-bson node module and if it's at least
  close to as fast as v8ToMongo, get rid of MongoV8Helpers. Otherwise, rename
  MongoV8Helpers to someone meaningful (all it does/should do is convert from
  V8 types <-> BSON).
* Fix missing initializer that is causing runGlobalInitializers() to fail.
* Provide our own DocumentSource based on node streams or something so we can
  pass items through a pipeline as they become available.  This is low priority
  because $group and $sort will force the entire pipeline to be synchronous
  anyways.
* Make a base class for MongoDB's scripting V8Scope class
  (in mongo/scripting/engine_v8.*) that we can use for converting V8 object to
  and from BSON.  Update: hoping this will no longer be needed.
  - Make the relevant items in src/mongo/scripting/v8_db.* either take the new
    base class instead of Scope* or make them templates so they can accept
    either (as long as in MongoDB and in our code only one set ever get
    instantiated.
* Remove #if 0 blocks from MongoV8Helpers.
* Rename v8ToMongo* to v8ToBson*.
* See if it would be easy to add a pipeline-version flag [2.4, 2.6].
