#include <sstream>

#include <node.h>
#include <v8.h>


//TODO: make our own version of this to help setup stuff for mongo (esp. using node's v8 and doing using namespace ...)
//#include "mongo/pch.h"

//TODO: include the appropriate files.  Note: these are found in the `foundation` library along with StartupTest.
#define verify(x) {}
#define uassert(a,b,c) {}
#define massert(a,b,c) {}
#define dassert(a) {}			//src/mongo/util/assert_util.h

#include "mongo/base/initializer.h"  //runGlobalInitializers()
#include "mongo/bson/bsonobjbuilder.h"

//#include <mongo/db/json.h> //fromjson().

//Pipeline stuff.

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

	//mongo::BSONObj obj;

	//namespace mmb = mongo::mutablebson;
	//mmb::Document doc(obj);

	//mongo::BSONObjBuilder bldr;
	//bldr << "$limit" << 2;

	//mongo::BSONObj test(BSON("hello" << bldr.obj()));
	//return scope.Close(String::New(test.jsonString().c_str()));

	//mongo::BSONArray arr = BSON_ARRAY( BSON( "bar" << 1 ) << "baz" << "qux" );
	//return scope.Close(String::New(arr.jsonString().c_str()));

	//mongo::BSONArray arr = BSON_ARRAY( BSON( "bar" << 1 ) << "baz" << "qux" );

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

//std::cout << "  cmd.jsonString()=" << cmd.jsonString() << std::endl;
//std::cout << "  cmd.dump()=\n"; cmd.dump(); std::cout << std::endl;

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


	//mongo::BSONArrayBuilder arrBldr(array->Length()),
//	mongo::BSONArrayBuilder arrBldr;//,
							//& arrBldrTop(arrBldr);
	//mongo::BSONObj pipeline(converter.v8ToMongo(array)); //Yay!  So close!
	//arrBldr.append(argsBson);
	//mongo::BSONArrayBuilder arrBldrTop,
	//						arrBldr(arrBldrTop.subarrayStart());
/*	for (uint32_t i = 0; i < array->Length(); ++i) {
		Local<Value> element(array->Get(i));

std::cout << "  element.ToObject()=" << *v8::String::Utf8Value(element->ToObject()->ToDetailString()) << std::endl;
std::cout << "  element.ToInteger()=" << *v8::String::Utf8Value(element->ToInteger()->ToDetailString()) << std::endl;
		mongo::BSONObj temp(converter.v8ToMongo(element->ToObject()));

std::cout << "  converted obj=" << temp.jsonString() << std::endl;
//std::cout << "  converted type=" << temp.firstElementType() << ", obj=" << temp.jsonString() << std::endl;

		arrBldr.append(temp);
		//arrBldr.append(mongo::BSONObjBuilder::numStr(i), temp);

		//arrBldr.append(i);//temp);  //Was for a simple test only!
		//arrBldr.append(converter.v8ToMongo(element->ToObject()));
	}
*/
////std::cout << "  arrBldrTop.IsArray()=" << arrBldrTop.isArray() << std::endl;
//std::cout << "  arrBldr.IsArray()=" << arrBldr.isArray() << std::endl;
	//TODO: release array.

	//mongo::BSONArray pipelineArr(arrBldr.arr());
	//mongo::BSONObj pipeline(pipelineArr);
	//mongo::BSONObj pipeline(arrBldr.obj());
	//mongo::BSONObj pipeline(arrBldrTop.done());

	//This results in an actual array when using just a BSONArrayBuilder. :)
	//mongo::BSONObjBuilder temp;
	//temp.append("foo", arrBldr.arr());
	//mongo::BSONObj pipeline(temp.obj());

	//mongo::BSONObj pipeline(mongo::fromjson("{\"t\":[10,{\"hello\":\"world\"}]}"));
/*
std::cout << "  pipeline.toString()=" << pipeline.toString() << std::endl;
std::cout << "  pipeline.isValid()=" << pipeline.isValid() << std::endl;
std::cout << "  pipeline.couldBeArray()=" << pipeline.couldBeArray() << std::endl;
std::cout << "  pipeline.dump()=\n"; pipeline.dump(); std::cout << std::endl;
*/
	//return scope.Close(String::New(result.obj().jsonString().c_str()));
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

/*
mongo::BSONObj toBSON(const Local<Value>& in) {

	//TODO: optimize this by going directly from the v8 type to BSON.

	

}

mongo::BSONObj ToBSON(const Local<Array>& in) {
	//TODO.
}

mongo::BSONObj ToBSON(const Local<Object>& in) {
	//TODO.
}
*/

/*
mongo::BSONObj ToBSON(const Local<Array>& in) {
	//TODO.
}

mongo::BSONObj ToBSON(const Local<Object>& in) {
	//TODO.
}


//I started this from bson2v8 but the original code needs quite a bit of reworking.  Going to just JSON.strigify() and mongo::parsejson() for now.
mongo::BSONObj ToBSON(const Local<Value>& in) {

	//Check type and then call one of the above.

	//-----

	//Refactor this and move into the appropriate function above.

	int ownsData = out->ownsData;
	uint32_t keys_len = 0;
	Local<Array> keys = in.GetOwnPropertyNames();

	if (in->IsArray()) {
		keys_len = Local<Array>::Cast(in)->Length();
	} else if (in->IsObject()) {
		keys_len = in.GetOwnPropertyNames();
	} else {
		//TODO: handle or exit horribly.
	}

	for (uint32_t i=0; i<keys_len; i++) {
		char* key; Local<Value> value;
		if (in->IsArray()) {
			std::ostringstream oss; oss << i;
			key = const_cast<char*>(oss.str().c_str());
			value = Local<Object>::Cast(in)->Get(i);
		}
		else if (in->IsObject()) {
			key = GetCStringFromV8String(keys->Get(i), "");
			value = Local<Object>::Cast(in)->Get(V8Symbol(key));
		};

		if (value->IsString() || value->IsStringObject()) {
			bson_append_string(out, key, GetCStringFromV8String(value, ""));
		}
		else if (value->IsNumber() || value->IsNumberObject()) {
			bson_append_int(out, key, value->Uint32Value());
		}
		else if (value->IsTrue()) {
			bson_append_bool(out, key, true);
		}
		else if (value->IsFalse()) {
			bson_append_bool(out, key, false);
		}
		else if (value->IsDate()) {
			bson_append_date(out, key, Local<Date>::Cast(value)->NumberValue());
		}
		else if (value->IsArray()) {
			bson_append_start_array(out, key);
			ToBSON(value, out);
			bson_append_finish_array(out);
		}
		else if (value->IsObject()) {
			bson_append_start_object(out, key);
			ToBSON(value, out);
			bson_append_finish_object(out);
		};
	};
	// finish this bson
	if (1 != ownsData) bson_finish(out);
}
*/

NODE_MODULE(pipeline, init)
