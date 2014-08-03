#include <sstream>

#include <node.h>
#include <v8.h>


//TODO: make our own version of this to help setup stuff for mongo (esp. using node's v8 and doing using namespace ...)
//#include "mongo/pch.h"

//TODO: include the appropriate files.  Note: these are found in the `foundation` library along with StartupTest.
#define verify(x) {}
#define uassert(a,b,c) {}
#define massert(a,b,c) {}
#define dassert(a) {}						//src/mongo/util/assert_util.h

#include "mongo/pch.h"

#include "mongo/base/initializer.h" 	 	//runGlobalInitializers()
#include "mongo/bson/bsonobjbuilder.h"

//Put this in our "mongo_pch.h" and include it before all mongo things.
#include <boost/smart_ptr/intrusive_ptr.hpp>
using boost::intrusive_ptr;

//#include "mongo/db/pipeline/document_source.h"
#include "mongo/db/pipeline/expression_context.h"
#include "mongo/db/pipeline/pipeline.h"

#include "mongo-ours/db/interrupt_status_noop.h"
//TODO: use this instead of converting the input javascript types to BSON up front.
#include "mongo-ours/db/pipeline/document_source_v8.h"

#include "MongoV8Helpers.h"


using namespace v8;

namespace mongo {
	//Yet another dynamic symbol failure (when using $sort).
	//This is defined in mongo/db/server_options_helpers.cpp providing my own version
	//to avoid having to pull in even more libs.  Should move this somewhere else.
	bool isMongos() { return false; }
}

Handle<Value> aggregate(const Arguments& args) {
	HandleScope scope;

	if (!args[0]->IsArray()) {
		//throw new ...
		return scope.Close(String::New("BAD PIPELINE, MUST BE AN ARRAY!"));
	}

	if (!args[1]->IsArray()) {
		//throw new ...
		return scope.Close(String::New("BAD DOCUMENT STREAM, MUST BE AN ARRAY!"));
	}

	not_mongo::MongoV8Helpers converter;

	//1) Build a bson version of the pipeline :)
	Handle<Array> v8Pipeline = Handle<Array>::Cast(args[0]);
	mongo::BSONArray pipeline(converter.v8ToMongo(v8Pipeline));

	mongo::BSONObjBuilder bldr;
	bldr.append("pipeline", pipeline);

	mongo::BSONObj cmd(bldr.obj());

	boost::intrusive_ptr<mongo::ExpressionContext> ctx =
		new mongo::ExpressionContext(mongo::InterruptStatusNoop::status,
				mongo:: NamespaceString("node-pipeline"));

	try {
		std::string errmsg;
		boost::intrusive_ptr<mongo::Pipeline> aggregator =
			mongo::Pipeline::parseCommand(errmsg, cmd, ctx);

		if (!aggregator.get())
			return scope.Close(String::New("{\"error\": \"parsing pipeline failed.\"}")); // Parsing error.

		//These steps were pieced together from:
		//  PipelineD::prepareCursorSource(aggregator, pCtx);

		//TODO: I've skipped a bunch of steps like coalescing just to test this out, put them back in.

		//TODO: make our own version of DocumentSourceBsonArray so we do not have to
		//convert the entire array to BSON upfront, we can do it an element at time.
		Handle<Array> v8Input = Handle<Array>::Cast(args[1]);

		boost::intrusive_ptr<mongo::DocumentSourceV8> inputSrc(
			mongo::DocumentSourceV8::create( v8Input, ctx ) );
		aggregator->addInitialSource(inputSrc);

		//Note: DocumentSourceOut and DocumentSourceGeoNear 'implement' DocumentSourceNeedsMongod
		//So they are not allowed right now.  I haven't looked much at MongodImplementation but we can
		//probably support at least Geo and maybe our own version of $out.

		aggregator->stitch();

		/*if (aggregator->isExplain()) {
			result << "stages" << Value(aggregator->writeExplainOps());
		return true; // don't do any actual execution
		}*/

		//Run w/o cursor (get all of the results at once).
		mongo::BSONObjBuilder result;
		aggregator->run(result);

		//TODO: use MongoToV8 to convert the results to javascript.
		//      Also try using JSON.parse() and compare the performance of the two for large result sets.
		//Yikes!  'result.obj().jsonString()' takes ~260ms for the big subtract test.
		return scope.Close(String::New(result.obj().jsonString().c_str()));
	} catch(mongo::UserException& e) {
		return scope.Close(String::New((std::string("{\"error\": \"running the pipeline failed.\", \"message\":\"") + e.what() + std::string("\"}")).c_str())); // Parsing error.
	}

	//See db/commands/pipeline_command.cpp handleCursorCommand() and PipelineRunner
		//for how to get result documents back as they are processed.


/*
		//Async output.
        boost::optional<BSONObj> getNextBson() {
            if (boost::optional<Document> next = _pipeline->output()->getNext()) {
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
	const char** argv(0), ** envp(0);

    mongo::Status status(mongo::runGlobalInitializers(argc, argv, envp));
    if (!status.isOK()) {
		std::stringstream msg;
        msg << "Failed global initialization: " << status;
		ThrowException(Exception::Error(String::New(msg.str().c_str())));
		return;
    }

	exports->Set(String::NewSymbol("aggregate"), FunctionTemplate::New(aggregate)->GetFunction());
}

NODE_MODULE(pipeline, init)
