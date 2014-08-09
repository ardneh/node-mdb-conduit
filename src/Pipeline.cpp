#include <sstream>

#include <node.h>
#include <v8.h>

// TODO: make our own version of this to help setup stuff for mongo (esp. using
// node's v8 and doing using namespace ...)
//#include "mongo/pch.h"

// TODO: include the appropriate files.  Note: these are found in the
// `foundation` library along with StartupTest.
#define verify(x)                                                              \
  {}
#define uassert(a, b, c)                                                       \
  {}
#define massert(a, b, c)                                                       \
  {}
#define dassert(a)                                                             \
  {} // src/mongo/util/assert_util.h

#include "mongo/pch.h"

#include "mongo/base/initializer.h" //runGlobalInitializers()
#include "mongo/bson/bsonobjbuilder.h"

// Put this in our "mongo_pch.h" and include it before all mongo things.
#include <boost/smart_ptr/intrusive_ptr.hpp>
using boost::intrusive_ptr;

//#include "mongo/db/pipeline/document_source.h"
#include "mongo/db/pipeline/expression_context.h"
#include "mongo/db/pipeline/pipeline.h"

#include "mongo-ours/db/interrupt_status_noop.h"
// TODO: use this instead of converting the input javascript types to BSON up
// front.
// This pulls in a BUNCH of stuff at compile time :/
#include "mongo-ours/db/pipeline/document_source_v8.h"

#include "MongoV8Helpers.h"

using namespace v8;

// Begin hacks to get around missing symbols.

namespace mongo {
// Yet another dynamic symbol failure (when using $sort).
// This is defined in mongo/db/server_options_helpers.cpp providing my own
// version
// to avoid having to pull in even more libs.  Should move this somewhere else.
bool isMongos() { return false; }

// Yep, this is horrible. :)  It's defined as part of the mongos code
// and I do not want to pull any more things in.
struct VersionManager {};
VersionManager versionManager;
}

// Begin hacks to make sure the linker knows we actually use these.

#include <db/matcher/expression_parser.h>

namespace mongo {

StatusWithMatchExpression (*weNeed_parse)(const BSONObj &) =
    &MatchExpressionParser::parse;
}

// Done with hacks... For now!

Handle<Value> callWithError(HandleScope &scope, Local<Function> &callback,
                            const char *const msg) {
  Local<Value> error(Exception::TypeError(String::New(msg)));
  callback->Call(Context::GetCurrent()->Global(), 1, &error);
  return scope.Close(Undefined());
}

Handle<Value> aggregate(const Arguments &args) {
  // TODO: use boost wrapper for scoped enums.
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
    // throw new ...
    ThrowException(
        Exception::TypeError(String::New("Third argument must be a callback")));
    return scope.Close(Undefined());
  }

  Local<Function> callback(Local<Function>::Cast(args[Args::CALLBACK]));

  if (!args[Args::PIPELINE]->IsArray()) {
    return callWithError(scope, callback, "BAD PIPELINE, MUST BE AN ARRAY!");
    // Local<Value> error(Exception::TypeError(String::New("BAD PIPELINE, MUST
    // BE AN ARRAY!")));
    // callback->Call(Context::GetCurrent()->Global(), 1, &error);
    // return scope.Close(Undefined());
  }

  if (!args[Args::DOCUMENTS]->IsArray()) {
    return callWithError(scope, callback,
                         "BAD DOCUMENT STREAM, MUST BE AN ARRAY!");
  }

  not_mongo::MongoV8Helpers converter;

  // 1) Build a bson version of the pipeline :)
  Handle<Array> v8Pipeline = Handle<Array>::Cast(args[0]);
  mongo::BSONArray pipeline(converter.v8ToMongo(v8Pipeline));

  mongo::BSONObjBuilder bldr;
  bldr.append("pipeline", pipeline);

  mongo::BSONObj cmd(bldr.obj());

  boost::intrusive_ptr<mongo::ExpressionContext> ctx =
      new mongo::ExpressionContext(mongo::InterruptStatusNoop::status,
                                   mongo::NamespaceString("node-pipeline"));

  try {
    std::string errmsg;
    boost::intrusive_ptr<mongo::Pipeline> aggregator =
        mongo::Pipeline::parseCommand(errmsg, cmd, ctx);

    if (!aggregator.get()) {
      return callWithError(scope, callback, "Could not parse the pipeline.");
    }

    // These steps were pieced together from:
    //  PipelineD::prepareCursorSource(aggregator, pCtx);

    // TODO: I've skipped a bunch of steps like coalescing just to test this
    // out, put them back in.

    // TODO: make our own version of DocumentSourceBsonArray so we do not have
    // to
    // convert the entire array to BSON upfront, we can do it an element at
    // time.
    Handle<Array> v8Input = Handle<Array>::Cast(args[1]);

    boost::intrusive_ptr<mongo::DocumentSourceV8> inputSrc(
        mongo::DocumentSourceV8::create(v8Input, ctx));
    aggregator->addInitialSource(inputSrc);

    // Note: DocumentSourceOut and DocumentSourceGeoNear 'implement'
    // DocumentSourceNeedsMongod
    // So they are not allowed right now.  I haven't looked much at
    // MongodImplementation but we can
    // probably support at least Geo and maybe our own version of $out.

    aggregator->stitch();

    /*if (aggregator->isExplain()) {
            result << "stages" << Value(aggregator->writeExplainOps());
    return true; // don't do any actual execution
    }*/

    // Run w/o cursor (get all of the results at once).
    mongo::BSONObjBuilder result;
    aggregator->run(result);

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
  catch (mongo::UserException &e) {
    return callWithError(scope, callback, e.what());
  }

  // See db/commands/pipeline_command.cpp handleCursorCommand() and
  // PipelineRunner
  // for how to get result documents back as they are processed.

  /*
                  //Async output.
          boost::optional<BSONObj> getNextBson() {
              if (boost::optional<Document> next =
     _pipeline->output()->getNext()) {
                  if (_includeMetaData) {
                      return next->toBsonWithMetaData();
                  }
                  else {
                      return next->toBson();
                  }
              }

              return boost::none;
          }
  */
  return scope.Close(String::New("{\"error\": \"should never get here.\"}"));
}

void init(Handle<Object> exports) {

  const int argc(0);
  const char **argv(0), **envp(0);

  mongo::Status status(mongo::runGlobalInitializers(argc, argv, envp));
  if (!status.isOK()) {
    std::stringstream msg;
    msg << "Failed global initialization: " << status;
    ThrowException(Exception::Error(String::New(msg.str().c_str())));
    return;
  }

  exports->Set(String::NewSymbol("aggregate"),
               FunctionTemplate::New(aggregate)->GetFunction());
}

NODE_MODULE(pipeline, init)
