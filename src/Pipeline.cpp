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


using namespace v8;

Handle<Value> Method(const Arguments& args) {
	HandleScope scope;

	//mongo::BSONObj obj;

	//namespace mmb = mongo::mutablebson;
	//mmb::Document doc(obj);

	mongo::BSONObjBuilder tsRestrictBldr;
	tsRestrictBldr << "$gt" << 2;
	tsRestrictBldr << "$lt" << 5;

	mongo::BSONObj test(BSON("ts" << tsRestrictBldr.obj()));

	return scope.Close(String::New(test.jsonString().c_str()));
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("hello"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(pipeline, init)
