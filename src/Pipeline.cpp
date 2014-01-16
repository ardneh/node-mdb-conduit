#include <node.h>
#include <v8.h>

//TODO: include the appropriate files.  Note: these are found in the `foundation` library along with StartupTest.
#define verify(x) {}
#define uassert(a,b,c) {}
#define massert(a,b,c) {}

//TODO: remove need for this by only included the sources we need.
#include <mongo/db/server_parameters.h>

#include <mongo/bson/bsonobjbuilder.h>
//#include <mongo/bson/bson.h>
//#include <mongo/bson/mutable/document.h>

#include <mongo/db/json.h> //fromjson().

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
	mongo::BSONArrayBuilder arrBldr;
	Handle<Array> array = Handle<Array>::Cast(args[0]);
	for (uint32_t i = 0; i < array->Length(); ++i) {
		Local<Value> element(array->Get(i));

		arrBldr.append(converter.v8ToMongo(element->ToObject()));
	}

	//TODO: release array.

	mongo::BSONArray pipeline(arrBldr.arr());
	//mongo::BSONObj test(mongo::fromjson("[{\"hello\":\"world\"}]"));

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
