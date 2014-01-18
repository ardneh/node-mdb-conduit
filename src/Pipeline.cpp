#include <node.h>
#include <v8.h>


//TODO: make our own version of this to help setup stuff for mongo (esp. using node's v8 and doing using namespace ...)
//#include "mongo/pch.h"

//TODO: include the appropriate files.  Note: these are found in the `foundation` library along with StartupTest.
#define verify(x) {}
#define uassert(a,b,c) {}
#define massert(a,b,c) {}
#define dassert(a) {}			//src/mongo/util/assert_util.h

//TODO: remove need for this by only included the sources we need.
//#include <mongo/db/server_parameters.h>

#include <mongo/bson/bsonobjbuilder.h>
//#include <mongo/bson/bson.h>
//#include <mongo/bson/mutable/document.h>

#include <mongo/db/json.h> //fromjson().

//Pipeline stuff.

//Put this in our "mongo_pch.h" and include it before all mongo things.
#include <boost/smart_ptr/intrusive_ptr.hpp>
using boost::intrusive_ptr;

#include "mongo-ours/db/interrupt_status_noop.h"
//#include "mongo/db/pipeline/document.h"
#include "mongo/db/pipeline/expression_context.h"
//#include "mongo/db/pipeline/field_path.h"
#include "mongo/db/pipeline/pipeline.h"

#include "MongoV8Helpers.h"

namespace mongo {
	
	class PeriodicTask {};

}

using namespace v8;

Handle<Value> Method(const Arguments& args) {
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
	Handle<Array> array = Handle<Array>::Cast(args[0]);
	mongo::BSONArray pipeline(converter.v8ToMongo(array));

	mongo::BSONObjBuilder bldr;
	bldr.append("pipeline", pipeline);

	mongo::BSONObj cmd(bldr.obj());

//std::cout << "  cmd.jsonString()=" << cmd.jsonString() << std::endl;
//std::cout << "  cmd.dump()=\n"; cmd.dump(); std::cout << std::endl;

	boost::intrusive_ptr<mongo::ExpressionContext> ctx =
		new mongo::ExpressionContext(mongo::InterruptStatusNoop::status,
				mongo:: NamespaceString("node-pipeline"));

	std::string errmsg;
	boost::intrusive_ptr<mongo::Pipeline> aggregator =
		mongo::Pipeline::parseCommand(errmsg, cmd, ctx);



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
	return scope.Close(String::New(pipeline.jsonString().c_str()));
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("hello"), FunctionTemplate::New(Method)->GetFunction());
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
