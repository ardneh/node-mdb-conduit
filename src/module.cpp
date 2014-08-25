/**
Provide mdb-aggregate, a MongoDB style aggregation pipeline, to NodeJS.

Copyright (C) 2014  Charles J. Ezell III

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <sstream>
#include <vector>

#include <boost/scoped_array.hpp>

#include <node.h>
#include <v8.h>

#include <mdb-conduit/src/mdb_pipeline.h>
#include <mdb-conduit/src/tools/mdb_conduit.h>

// TODO: use this instead of converting the input javascript types to BSON up
// front.  Unfortunately this pulls in a BUNCH of stuff at compile time :/
#include "db/pipeline/document_source_v8.h"

#include "MongoV8Helpers.h"


using namespace std;
using namespace v8;

//Wrap up boilerplate for error handling.
Handle<Value> callWithError(HandleScope &scope, Local<Function> &callback,
                            const char *const msg) {
  Local<Value> error(Exception::TypeError(String::New(msg)));
  callback->Call(Context::GetCurrent()->Global(), 1, &error);
  return scope.Close(Undefined());
}

Handle<Value> aggregate(const Arguments &args) {
   struct Args {
      enum {
         PIPELINE,
         DOCUMENTS,
         CALLBACK,
         numArgs
      };
   };

  HandleScope scope;

  if (args.Length() < Args::numArgs) {
    ThrowException(
        Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[Args::CALLBACK]->IsFunction()) {
    ThrowException(
        Exception::TypeError(String::New("Third argument must be a callback")));
    return scope.Close(Undefined());
  }

  Local<Function> callback(Local<Function>::Cast(args[Args::CALLBACK]));

  if (!args[Args::PIPELINE]->IsArray()) {
    return callWithError(scope, callback, "BAD PIPELINE, MUST BE AN ARRAY!");
  }

  if (!args[Args::DOCUMENTS]->IsArray()) {
    return callWithError(scope, callback,
                         "BAD DOCUMENT STREAM, MUST BE AN ARRAY!");
  }

  not_mongo::MongoV8Helpers converter;

  // 1) Build a bson version of the pipeline :)
  Handle<Array> v8Pipeline = Handle<Array>::Cast(args[0]);
  mongo::BSONArray pipeline(converter.v8ToMongo(v8Pipeline));

  try {
     conduit::Pipeline conduit(pipeline);

    Handle<Array> v8Input = Handle<Array>::Cast(args[1]);

    boost::intrusive_ptr<mongo::DocumentSourceV8> inputSrc(
        mongo::DocumentSourceV8::create(v8Input, conduit.getContext()));

    // Run w/o cursor (get all of the results at once).
    mongo::BSONObjBuilder result;
    conduit(inputSrc, result);

    // TODO: use MongoToV8 to convert the results to javascript.
    //      Also try using JSON.parse() and compare the performance of the two
    // for large result sets.
    // Yikes!  'result.obj().jsonString()' takes ~260ms for the big subtract
    // test.
    // Update: recent tests are around ~60ms.  Not sure if it's improved that
    // much
    // or if me test changed...
    Local<Value> results[2] = { Local<Value>::New(Undefined()),
                                Local<Value>::New(String::New(
                                    result.obj().jsonString().c_str())) };

      callback->Call(Context::GetCurrent()->Global(), 2, results);
      return scope.Close(Undefined());
   }
   catch (std::exception &e) {
      return callWithError(scope, callback, e.what());
   }
}

Handle<Value> _conduit_main(const Arguments &args) {

   struct Args {
      enum {
         ARGV,
         ENV,
         numArgs
      };
   };

  HandleScope scope;

   if (args.Length() < Args::numArgs) {
      scope.Close(Undefined());

      ThrowException(
         Exception::TypeError(String::New("_conduit_main(): Wrong number of arguments")));
   }

   if (!args[Args::ARGV]->IsArray()) {
      scope.Close(Undefined());

      ThrowException(
         Exception::TypeError(String::New("_conduit_main(): Param 1, argv should be an array of strings.")));
   }

   if (!args[Args::ENV]->IsObject()) {
      scope.Close(Undefined());

      ThrowException(
         Exception::TypeError(String::New("_conduit_main(): Param 2, env should be an object.")));
   }

   Local<Array> v8Argv(Local<Array>::Cast(args[Args::ARGV]));
   int argc(v8Argv->Length());

   //Setup parallel arrays.  The first will hold the data for the args, while
   //the second will just point into that memory.
   vector<vector<char>> argvHolder;
   boost::scoped_array<char*> argv(new char*[argc]);

   unsigned int i(0);
   for (unsigned int length=v8Argv->Length(); i < length; ++i) {
      String::Utf8Value buf(v8Argv->Get(i)->ToString());

      argvHolder.push_back(vector<char>(*buf, *buf + buf.length() + 1));
      argv[i] = &argvHolder[i][0];
   }

   //for(int idx(0); idx < argc; ++idx) {
   //   cout << "Arg " << idx << ": " << argv[idx] << '\n';
   //}

   char** env(nullptr);
   const int result(conduit::conduit_main(argc, argv.get(), env));

   return scope.Close(Number::New(result));
}

void init(Handle<Object> exports) {

   int argc(0);
   char** argv(nullptr), **env(nullptr);

   mongo::Status status(conduit::intialize_module(argc, argv, env));
   if (!status.isOK()) {
      std::stringstream msg;
      msg << "Failed initialization: " << status;

      ThrowException(Exception::Error(String::New(msg.str().c_str())));
      return;
   }

  exports->Set(
      String::NewSymbol("aggregate"),
      FunctionTemplate::New(aggregate)->GetFunction()
   );

   exports->Set(
      String::NewSymbol("_conduit_main"),
      FunctionTemplate::New(_conduit_main)->GetFunction()
   );
}

//TODO: use context aware version.
NODE_MODULE(mdb_conduit, init)
