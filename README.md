This node modules makes [MongoDB's](http://mongodb.org) aggregation pipeline
(henceforce `pipeline`) available for use in NodeJS.

Potential uses
------------------
* Provide a common API to query data in an application (one DSL).
* Pre/post-processing of data returned by MongoDB.
* Test out ideas for new pipeline functionality.
* Profile pipeline operations more easily.

Disclaimers
---------------
* Full text search and where expressions probably don't work. See
  third_party/mdb_conduit/README.md for more details.
* This was initially put together as a very quick proof-of-concept so in it's
  current state it is not pretty.
* It is only going to build on `nix platforms for now.  "Linux" is whitelisted
  and OS X should be easy to make work when that becomes a priority (soonish).
* This is also my first native node module so it's going to be rough.

Build Requirements
-------------------------
* A bash compatible shell

Development Build Steps
---------------------------------------
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
  ```sh
  npm install && npm test
  ```

Troubleshooting
--------------------------
If you get a 'Error: Failed global initialization: BadValue default' error
right after Module.load this most likely means that a function wasn't linked
in that was needed (because I'm playing games).  Build the module in debug
mode (node-gyp --debug configure build) and set a breakpoint in
build/Debug/obj.target/mongo/base/initializer_dependency_graph.cpp on
line 113.  Then look currentNode.first and currentNode.second to see what
initializer function is missing from the the mdb-conduit shared library.
