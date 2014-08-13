This node modules makes [MongoDB's](http://mongodb.org) aggregation pipeline
(henceforce `pipeline`) available for use in NodeJS.

Potential uses
------------------
* Provide a common API to query data in an application (one DSL).
* Post-processing of data returned by MongoDB.
* Pre-processing of pipelines sent to MongoDB.
* Test out ideas for new pipeline functionality.
* Profile pipeline operations more easily.

Disclaimers
---------------
* Full text search pulled in a bunch of stuff to read from the disk so I
  assume it will not work.  It's not something I anticipate needing any time
  soon so it's very low on the list of things to test/fix.
* Where expressions require a bunch of auth stuff.  I doubt they will work.
* This was initially put together as a very quick proof-of-concept so in it's
  current state it is not pretty.
* It is only going to build on `nix platforms for now.  "Linux" should work
  and OS X should be easy to make work if it does not already.
* This is the first time I've used gyp, so I am certain I could be doing some
  things better there.
* This is also my first native node module.  I want to be to compatible with
  v0.10 and v0.11/12.

Build Requirements
-------------------------
* Python
* scons
* The rest of the MongoDB build requirements
*  See [Building MongoDB](http://www.mongodb.org/about/contributors/tutorial/build-mongodb-from-source/)

Development Build Steps
---------------------------------------
Note: All of these will be rolled into bindings.gyp as time permits
* Setup a nodeenv.  You can skip this if you want to install globally, etc...
  ```sh
  pip install nodeenv
  nodeenv --force nodeenv
  . nodeenv/bin/activate
  npm install -g node-gyp
  ```
* Clone the MongoDB source:
  ```sh
  git clone -b v2.6 --depth 2 --single-branch https://github.com/mongodb/mongo src/third-party/mongo
   ```
* Make sure the handful of generated files we need are created.
  ```sh
  # cd src/third-party/mongo
  patch -p1 < ../patches/mongo/do_not_autoclean.patch
  # TODO: update this.  A full build isn't needed, just a few steps to make
  # sure error_codes.h/cpp and action_type.h/cpp are generated.
  cd -
  ```
* Now we can build the actual node module:
  ```sh
  npm install -g --link
  ```

TODO
--------
* Move all of the pipeline code to it's own repo and have this download and
  build it.  The only code that should be in this package is the node glue.
*  Compile MongoDB's buildinfo.cpp and
   call compilerFlags() in bindings.gyp to get the compiler flags for the
   pipeline.
* Move compile the mongo code in its own target so different cflags can be
   used.
* Run as many of MongoDB's unit tests as possible.
* Develop an pipeline agnostic set of tests that can be used for this and munge.
* Test out the C++ driver's json parser!  See if that is faster than going from
  v8 objects to bson.
*   See if maybe another version of it could be used to go from v8 to bson.
* Improve speed.  Re-time against mungedb-aggregate, but against bson data
  (no-conversion step).
*  Investigate what it would take for the bson builder / reader classes to
   handle json.
* Make it able to accept v8 objects, json and bson.
* Use thread pool instead of trying to make any part of the pipeline async,
  which is pretty much pointless because of $group and $sort).
* See if it would be easy to add a pipeline-version flag [2.4, 2.6].
* Use C++11 or, preferrably 14.
* Try the https://github.com/mongodb/js-bson node module and if it's at least
  close to as fast as v8ToMongo, get rid of MongoV8Helpers. Otherwise, rename
  MongoV8Helpers to someone meaningful (all it does/should do is convert from
  V8 types <-> BSON).
* Coalesce and optimize the input pipeline.
* Make it extendable (DocumentSources, Pipeline, etc...) preferably both from
  C++ AND node (much lower priority).
* Enable veryfy(), *assert(),
* Try to re-use MongoDB's pipeline test cases.
* Remove #if 0 blocks from MongoV8Helpers.
* Rename v8ToMongo* to v8ToBson*.
* Only include (build) the files we actually need from MongoDB (mostly in
  pipeline/).  document_source_cursor.h/cpp is causing us to pull in way more
  than we need.
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
