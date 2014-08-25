This node modules makes [MongoDB's](http://mongodb.org) aggregation pipeline
(henceforce `pipeline`) available for use in NodeJS.

Potential uses
--------------
* As a utility to explore bson dumps and/or transform json data.
* Provide a common API to query data in an application (one DSL).
* Pre/post-processing of data returned by MongoDB.
* Test out ideas for new pipeline functionality.
* Profile pipeline operations more easily.

Installation
------------
* ```sh
  npm install mdb-conduit
  ```
* There will be a mdb-conduit program installed in `node_modules/.bin`.

Examples
--------
* As a command.
  ```sh
   echo '[{v:5},{v:1},{v:3},{v:4},{v:2},{v:0}]' >> array.json
   mdb-conduit -e '[{$sort:{v:1}}]' array.json
  ```
* Programmatically.
  ```JavaScript
   var mdb_conduit = require('mdb-conduit'),
         pipeline = [{$sort:{v:1}}],
         docs = [{v:5},{v:1},{v:3},{v:4},{v:2},{v:0}];

   mdb_conduit.aggregate(pipeline, docs, function(err, results) {
      if(err) {
         console.error("Aggregation failed:", err);
         return;
      }

      console.log("result:", results)
   });
   ```

Disclaimers
-----------
* Full text search and where expressions probably don't work. See
  third_party/mdb_conduit/README.md for more details.
* It is only going to build on `nix platforms for now.  "Linux" is whitelisted
  and OS X should be easy to make work when that becomes a priority (soonish).

Build Requirements
------------------
* A bash compatible shell
* Git
* Python
* A C++11 capable compiler.

Development Build Steps
-----------------------
* Clone the module and cd into that directory.
* Setup a nodeenv.  You can skip this if you want to install globally, etc...
  ```sh
  pip install nodeenv
  nodeenv nodeenv
  . nodeenv/bin/activate
  ```
* Install these globally for convenience.
  ```sh
  npm install -g node-gyp mocha
  ```
* Build and test the module.  The build will take ~5-6 minutes on a slower box.
  Note: `npm install` runs bin/clean.py afterwards, removing all temporary build
  artifacts which you do _not_ want if you are going to do development on the
  module.
  ```sh
  node-gyp --debug configure build && npm test
  ```
* Debugging.
  ```sh
  gdb --args node your_pipeline_test.js
  ```
  Once inside gdb, do:
  ```
  set auto-solib-add
  b init
  r
  ```

Troubleshooting
---------------
If you get a 'Error: Failed global initialization: BadValue default' error
right after Module.load this most likely means that a function wasn't linked
in that was needed (because I'm playing games).  Build the module in debug
mode (node-gyp --debug configure build) and set a breakpoint in
build/Debug/obj.target/mongo/base/initializer_dependency_graph.cpp on
line 113.  Then look currentNode.first and currentNode.second to see what
initializer function is missing from the the mdb-conduit shared library.
