/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

//#include <vector>
#include <v8.h>

#include <boost/smart_ptr/scoped_ptr.hpp>

#include <mongo/bson/bson.h>
//#include <mongo/bson/bsonobjbuilder.h>

#include "mongo/util/concurrency/mutex.h"
#include "mongo/util/log.h"
//#include "mongo/logger/logstream_builder.h"
#include "mongo/base/disallow_copying.h"
#include "mongo/base/string_data.h"
//#include "mongo/client/dbclientinterface.h"
//#include "mongo/client/dbclientcursor.h"
#include "mongo/platform/unordered_map.h"
#include "mongo/util/concurrency/thread_name.h"
//#include "mongo/scripting/engine.h"
//#include "mongo/scripting/v8_deadline_monitor.h"
//#include "mongo/scripting/v8_profiler.h"

/**
 * V8_SIMPLE_HEADER must be placed in any function called from a public API
 * that work with v8 handles (and/or must be within the V8Scope's isolate
 * and context).  Be sure to close the handle_scope if returning a v8::Handle!
 */
#define V8_SIMPLE_HEADER                                                                      \
        v8::Locker v8lock(_isolate);          /* acquire isolate lock */                      \
        v8::Isolate::Scope iscope(_isolate);  /* enter the isolate; exit when out of scope */ \
        v8::HandleScope handle_scope;         /* make the current scope own local handles */  \
        v8::Context::Scope context_scope(_context); /* enter the context; exit when out of scope */

namespace not_mongo {

    class MongoV8Helpers;
    class BSONHolder;

    typedef v8::Handle<v8::Value> (*v8Function)(MongoV8Helpers* scope, const v8::Arguments& args);

    /**
     * The ObjTracker class keeps track of all weakly referenced v8 objects.  This is
     * required because v8 does not invoke the WeakReferenceCallback when shutting down
     * the context/isolate.  To track a new object, add an ObjTracker<MyObjType> member
     * variable to the V8Scope (if one does not already exist for that type).  Instead
     * of calling v8::Persistent::MakeWeak() directly, simply invoke track() with the
     * persistent handle and the pointer to be freed.
     */
    template <typename _ObjType>
    class ObjTracker {
    public:
        /** Track an object to be freed when it is no longer referenced in JavaScript.
         * @param  instanceHandle  persistent handle to the weakly referenced object
         * @param  rawData         pointer to the object instance
         */
        void track(v8::Persistent<v8::Value> instanceHandle, _ObjType* instance) {
            TrackedPtr* collectionHandle = new TrackedPtr(instance, this);
            _container.insert(collectionHandle);
            instanceHandle.MakeWeak(collectionHandle, deleteOnCollect);
        }
        /**
         * Free any remaining objects and their TrackedPtrs.  Invoked when the
         * V8Scope is destructed.
         */
        ~ObjTracker() {
            if (!_container.empty()) {
				using mongo::getThreadName; //Used by LOG macro.
				using mongo::LogstreamBuilder; //Used by LOG macro.
                LOG(1) << "freeing " << _container.size() << " uncollected "
                       << typeid(_ObjType).name() << " objects" << std::endl;
            }
            typename std::set<TrackedPtr*>::iterator it = _container.begin();
            while (it != _container.end()) {
                delete *it;
                _container.erase(it++);
            }
        }
    private:
        /**
         * Simple struct which contains a pointer to the tracked object, and a pointer
         * to the ObjTracker which owns it.  This is the argument supplied to v8's
         * WeakReferenceCallback and MakeWeak().
         */
        struct TrackedPtr {
        public:
            TrackedPtr(_ObjType* instance, ObjTracker<_ObjType>* tracker) :
                _objPtr(instance),
                _tracker(tracker) { }
            boost::scoped_ptr<_ObjType> _objPtr;
            ObjTracker<_ObjType>* _tracker;
        };

        /**
         * v8 callback for weak persistent handles that have been marked for removal by the
         * garbage collector.  Signature conforms to v8's WeakReferenceCallback.
         * @param  instanceHandle  persistent handle to the weakly referenced object
         * @param  rawData         pointer to the TrackedPtr instance
         */
        static void deleteOnCollect(v8::Persistent<v8::Value> instanceHandle, void* rawData) {
            TrackedPtr* trackedPtr = static_cast<TrackedPtr*>(rawData);
            trackedPtr->_tracker->_container.erase(trackedPtr);
            delete trackedPtr;
            instanceHandle.Dispose();
        }

        // container for all TrackedPtrs created by this ObjTracker instance
        std::set<TrackedPtr*> _container;
    };

    /**
     * 
     *
     */
    class MongoV8Helpers : boost::noncopyable {
    public:
        MongoV8Helpers();
        ~MongoV8Helpers();

#if 0
        virtual void init(const BSONObj* data);

        /**
         * Reset the state of this scope for use by another thread or operation
         */
        virtual void reset();

        /**
         * Terminate this scope
         */
        virtual void kill();

        /** check if there is a pending killOp request */
        bool isKillPending() const;

        /**
         * Connect to a local database, create a Mongo object instance, and load any
         * server-side js into the global object
         */
        virtual void localConnect(const char* dbName);

        virtual void externalSetup();

        virtual void installDBAccess();
#endif //Not needed.
        /*virtual*/ void installBSONTypes();

        /*virtual*/ std::string getError() { return _error; }

        /*virtual*/ bool hasOutOfMemoryException();
#if 0 //Not needed.

        /**
         * Run the garbage collector on this scope (native function).  @see GCV8 for the
         * javascript binding version.
         */
        void gc();

        /**
         * get a global property.  caller must set up the v8 state.
         */
        v8::Handle<v8::Value> get(const char* field);

        virtual double getNumber(const char* field);
        virtual int getNumberInt(const char* field);
        virtual long long getNumberLongLong(const char* field);
        virtual string getString(const char* field);
        virtual bool getBoolean(const char* field);
        virtual BSONObj getObject(const char* field);

        virtual void setNumber(const char* field, double val);
        virtual void setString(const char* field, const mongo::StringData& val);
        virtual void setBoolean(const char* field, bool val);
        virtual void setElement(const char* field, const mongo::BSONElement& e);
        virtual void setObject(const char* field, const BSONObj& obj, bool readOnly);
        virtual void setFunction(const char* field, const char* code);

        virtual int type(const char* field);

        virtual void rename(const char* from, const char* to);

        virtual int invoke(ScriptingFunction func, const BSONObj* args, const BSONObj* recv,
                           int timeoutMs = 0, bool ignoreReturn = false,
                           bool readOnlyArgs = false, bool readOnlyRecv = false);

        virtual bool exec(const mongo::StringData& code, const string& name, bool printResult,
                          bool reportError, bool assertOnError, int timeoutMs);

        // functions to create v8 object and function templates
        virtual void injectNative(const char* field, NativeFunction func, void* data = 0);
        void injectNative(const char* field, NativeFunction func, v8::Handle<v8::Object>& obj,
                          void* data = 0);

#endif //Not needed.
        // These functions inject a function (either an unwrapped function pointer or a pre-wrapped
        // FunctionTemplate) into the provided object. If no object is provided, the function will
        // be injected at global scope. These functions take care of setting the function and class
        // name on the returned FunctionTemplate.
        v8::Handle<v8::FunctionTemplate> injectV8Function(const char* name, v8Function func);
        v8::Handle<v8::FunctionTemplate> injectV8Function(const char* name,
                                                          v8Function func,
                                                          v8::Handle<v8::Object>& obj);
        v8::Handle<v8::FunctionTemplate> injectV8Function(const char* name,
                                                          v8::Handle<v8::FunctionTemplate> ft,
                                                          v8::Handle<v8::Object>& obj);

        // Injects a method into the provided prototype
        v8::Handle<v8::FunctionTemplate> injectV8Method(const char* name,
                                                        v8Function func,
                                                        v8::Handle<v8::ObjectTemplate>& proto);

        v8::Handle<v8::FunctionTemplate> createV8Function(v8Function func);
#if 0 //Not needed.
        virtual ScriptingFunction _createFunction(const char* code,
                                                  ScriptingFunction functionNumber = 0);
        v8::Local<v8::Function> __createFunction(const char* code,
                                                 ScriptingFunction functionNumber = 0);
#endif //Not needed.
        /**
         * Convert BSON types to v8 Javascript types
         */
        v8::Handle<v8::Object> mongoToLZV8(const mongo::BSONObj& m, bool readOnly = false);
        v8::Handle<v8::Value> mongoToV8Element(const mongo::BSONElement& f, bool readOnly = false);

        /**
         * Convert v8 Javascript types to BSON types
         */
        mongo::BSONObj v8ToMongo(v8::Handle<v8::Object> obj, int depth = 0);
        void v8ToMongoElement(mongo::BSONObjBuilder& b,
                              const mongo::StringData& sname,
                              v8::Handle<v8::Value> value,
                              int depth = 0,
                              mongo::BSONObj* originalParent = 0);
        void v8ToMongoObject(mongo::BSONObjBuilder& b,
                             const mongo::StringData& sname,
                             v8::Handle<v8::Value> value,
                             int depth,
                             mongo::BSONObj* originalParent);
        void v8ToMongoNumber(mongo::BSONObjBuilder& b,
                             const mongo::StringData& elementName,
                             v8::Handle<v8::Number> value,
                             mongo::BSONObj* originalParent);
        void v8ToMongoRegex(mongo::BSONObjBuilder& b,
                            const mongo::StringData& elementName,
                            v8::Handle<v8::RegExp> v8Regex);
        void v8ToMongoDBRef(mongo::BSONObjBuilder& b,
                            const mongo::StringData& elementName,
                            v8::Handle<v8::Object> obj);
        void v8ToMongoBinData(mongo::BSONObjBuilder& b,
                              const mongo::StringData& elementName,
                              v8::Handle<v8::Object> obj);
        mongo::OID v8ToMongoObjectID(v8::Handle<v8::Object> obj);

        v8::Local<v8::Value> newId(const mongo::OID& id);

        /**
         * Create a new function; primarily used for BSON/V8 conversion.
         */
        v8::Local<v8::Value> newFunction(const mongo::StringData& code);

		v8::Handle<v8::String> jsonStringify(v8::Handle<v8::Value> object);
		v8::Handle<v8::Object> jsonParse(v8::Handle<v8::Value> object);

        /**
         * Convert a JavaScript exception to a stl string.  Requires
         * access to the MongoV8Helpers instance to report source context information.
         */
        std::string v8ExceptionToSTLString(const v8::TryCatch* try_catch);

        /**
         * Create a V8 string with a local handle
         */
        static inline v8::Handle<v8::String> v8StringData(mongo::StringData str) {
            return v8::String::New(str.rawData(), str.size());
        }

        /**
         * Get the isolate this scope belongs to (can be called from any thread, but v8 requires
         *  the new thread enter the isolate and context.  Only one thread can enter the isolate.
         */
        v8::Isolate* getIsolate() { return _isolate; }

        /**
         * Get the JS context this scope executes within.
         */
        v8::Persistent<v8::Context> getContext() { return _context; }
        /**
         * Get the global JS object
         */
        v8::Persistent<v8::Object> getGlobal() { return _global; }

        // These are all named after the JS constructor name + FT
        v8::Handle<v8::FunctionTemplate> ObjectIdFT()       const { return _ObjectIdFT; }
        v8::Handle<v8::FunctionTemplate> DBRefFT()          const { return _DBRefFT; }
        v8::Handle<v8::FunctionTemplate> DBPointerFT()      const { return _DBPointerFT; }
        v8::Handle<v8::FunctionTemplate> BinDataFT()        const { return _BinDataFT; }
        v8::Handle<v8::FunctionTemplate> NumberLongFT()     const { return _NumberLongFT; }
        v8::Handle<v8::FunctionTemplate> NumberIntFT()      const { return _NumberIntFT; }
        v8::Handle<v8::FunctionTemplate> TimestampFT()      const { return _TimestampFT; }
        v8::Handle<v8::FunctionTemplate> MinKeyFT()         const { return _MinKeyFT; }
        v8::Handle<v8::FunctionTemplate> MaxKeyFT()         const { return _MaxKeyFT; }
        v8::Handle<v8::FunctionTemplate> MongoFT()          const { return _MongoFT; }
        v8::Handle<v8::FunctionTemplate> DBFT()             const { return _DBFT; }
        v8::Handle<v8::FunctionTemplate> DBCollectionFT()   const { return _DBCollectionFT; }
        v8::Handle<v8::FunctionTemplate> DBQueryFT()        const { return _DBQueryFT; }
        v8::Handle<v8::FunctionTemplate> InternalCursorFT() const { return _InternalCursorFT; }
        v8::Handle<v8::FunctionTemplate> LazyBsonFT()       const { return _LazyBsonFT; }
        v8::Handle<v8::FunctionTemplate> ROBsonFT()         const { return _ROBsonFT; }

		v8::Persistent<v8::Context> _context;
		v8::Persistent<v8::Object> _global;
		std::string _error;

        /// Like v8::Isolate* but calls Dispose() in destructor.
        class IsolateHolder {
            MONGO_DISALLOW_COPYING(IsolateHolder);
        public:
            IsolateHolder() :_isolate(NULL) {}
            ~IsolateHolder() {
                if (_isolate) {
                    _isolate->Dispose();
                    _isolate = NULL;
                }
            }

            void set(v8::Isolate* isolate) {
                mongo::fassert(17184, !_isolate);
                _isolate = isolate;
            }

            v8::Isolate* operator -> () const { return _isolate; };
            operator v8::Isolate* () const { return _isolate; };
        private:
            v8::Isolate* _isolate;
        };

		IsolateHolder _isolate; // NOTE: this must be destructed before the ObjTrackers

        // These are all named after the JS constructor name + FT
        v8::Persistent<v8::FunctionTemplate> _ObjectIdFT;
        v8::Persistent<v8::FunctionTemplate> _DBRefFT;
        v8::Persistent<v8::FunctionTemplate> _DBPointerFT;
        v8::Persistent<v8::FunctionTemplate> _BinDataFT;
        v8::Persistent<v8::FunctionTemplate> _NumberLongFT;
        v8::Persistent<v8::FunctionTemplate> _NumberIntFT;
        v8::Persistent<v8::FunctionTemplate> _TimestampFT;
        v8::Persistent<v8::FunctionTemplate> _MinKeyFT;
        v8::Persistent<v8::FunctionTemplate> _MaxKeyFT;
        v8::Persistent<v8::FunctionTemplate> _MongoFT;
        v8::Persistent<v8::FunctionTemplate> _DBFT;
        v8::Persistent<v8::FunctionTemplate> _DBCollectionFT;
        v8::Persistent<v8::FunctionTemplate> _DBQueryFT;
        v8::Persistent<v8::FunctionTemplate> _InternalCursorFT;
        v8::Persistent<v8::FunctionTemplate> _LazyBsonFT;
        v8::Persistent<v8::FunctionTemplate> _ROBsonFT;

        ObjTracker<BSONHolder> bsonHolderTracker;

        // See comments in strLitToV8
        typedef mongo::unordered_map<const char*, v8::Handle<v8::String> > StrLitMap;
        StrLitMap _strLitMap;

        template <size_t N>
        v8::Handle<v8::String> strLitToV8(const char (&str)[N]) {
            // Note that _strLitMap is keyed on string pointer not string
            // value. This is OK because each string literal has a constant
            // pointer for the program's lifetime. This works best if (but does
            // not require) the linker interns all string literals giving
            // identical strings used in different places the same pointer.

            StrLitMap::iterator it = _strLitMap.find(str);
            if (it != _strLitMap.end())
                return it->second;

            mongo::StringData sd (str, mongo::StringData::LiteralTag());
            v8::Handle<v8::String> v8Str = v8StringData(sd);

            // We never need to Dispose since this should last as long as MongoV8Helpers exists
            _strLitMap[str] = v8::Persistent<v8::String>::New(v8Str);

            return v8Str;
        }
#if 0 //Not needed.
        ObjTracker<DBClientWithCommands> dbClientWithCommandsTracker;
        ObjTracker<DBClientBase> dbClientBaseTracker;
        ObjTracker<DBClientCursor> dbClientCursorTracker;
#endif //Not needed.
    private:
        /**
         * Recursion limit when converting from JS objects to BSON.
         */
        static const int objectDepthLimit = 500;

        /**
         * Attach data to obj such that the data has the same lifetime as the Object obj points to.
         * obj must have been created by either LazyBsonFT or ROBsonFT.
         */
        void wrapBSONObject(v8::Handle<v8::Object> obj, mongo::BSONObj data, bool readOnly);

        v8::Persistent<v8::Function> _jsRegExpConstructor;

        /**
         * Trampoline to call a c++ function with a specific signature (MongoV8Helpers*, v8::Arguments&).
         * Handles interruption, exceptions, etc.
         */
        static v8::Handle<v8::Value> v8Callback(const v8::Arguments& args);

        v8::Persistent<v8::Object> _JSON;
        v8::Persistent<v8::Function> _jsonParse;
        v8::Persistent<v8::Function> _jsonStringify;
#if 0 //Not needed.
        /**
         * Interpreter agnostic 'Native Callback' trampoline.  Note this is only called
         * from v8Callback().
         */
        static v8::Handle<v8::Value> nativeCallback(MongoV8Helpers* scope, const v8::Arguments& args);

        /**
         * v8-specific implementations of basic global functions
         */
        static v8::Handle<v8::Value> load(MongoV8Helpers* scope, const v8::Arguments& args);
        static v8::Handle<v8::Value> Print(MongoV8Helpers* scope, const v8::Arguments& args);
        static v8::Handle<v8::Value> Version(MongoV8Helpers* scope, const v8::Arguments& args);
        static v8::Handle<v8::Value> GCV8(MongoV8Helpers* scope, const v8::Arguments& args);

        static v8::Handle<v8::Value> startCpuProfiler(MongoV8Helpers* scope, const v8::Arguments& args);
        static v8::Handle<v8::Value> stopCpuProfiler(MongoV8Helpers* scope, const v8::Arguments& args);
        static v8::Handle<v8::Value> getCpuProfile(MongoV8Helpers* scope, const v8::Arguments& args);

#endif //Not needed.
        /** Signal that this scope has entered a native (C++) execution context.
         *  @return  false if execution has been interrupted
         */
        bool nativePrologue();

        /** Signal that this scope has completed native execution and is returning to v8.
         *  @return  false if execution has been interrupted
         */
        bool nativeEpilogue();


        template <typename _HandleType>
        bool checkV8ErrorState(const _HandleType& resultHandle,
                               const v8::TryCatch& try_catch,
                               bool reportError = true,
                               bool assertOnError = true);

#if 0 //Not needed.
        /**
         * Register this scope with the mongo op id.  If executing outside the
         * context of a mongo operation (e.g. from the shell), killOp will not
         * be supported.
         */
        void registerOpId();

        /**
         * Unregister this scope with the mongo op id.
         */
        void unregisterOpId();

#endif //Not needed.
        mongo::mutex _interruptLock; // protects interruption-related flags
        bool _inNativeExecution;     // protected by _interruptLock
        bool _pendingKill;           // protected by _interruptLock
#if  0 //Not needed.
        int _opId;                   // op id for this scope
#endif //Not needed.
    };

    class BSONHolder {
    MONGO_DISALLOW_COPYING(BSONHolder);
    public:
        explicit BSONHolder(mongo::BSONObj obj) :
            //_scope(NULL),
            _obj(obj.getOwned()),
            _modified(false) {
            // give hint v8's GC
            v8::V8::AdjustAmountOfExternalAllocatedMemory(_obj.objsize());
        }
        ~BSONHolder() {
            //if (_scope && _scope->getIsolate())
                // if v8 is still up, send hint to GC
            //    v8::V8::AdjustAmountOfExternalAllocatedMemory(-_obj.objsize());
        }
        MongoV8Helpers* _scope;
        mongo::BSONObj _obj;
        bool _modified;
        bool _readOnly;
        std::set<std::string> _removed;
    };

    /**
     * Check for an error condition (e.g. empty handle, JS exception, OOM) after executing
     * a v8 operation.
     * @resultHandle         handle storing the result of the preceeding v8 operation
     * @try_catch            the active v8::TryCatch exception handler
     * @param reportError    if true, log an error message
     * @param assertOnError  if true, throw an exception if an error is detected
     *                       if false, return value indicates error state
     * @return true if an error was detected and assertOnError is set to false
     *         false if no error was detected
     */
    template <typename _HandleType>
    bool MongoV8Helpers::checkV8ErrorState(const _HandleType& resultHandle,
                                    const v8::TryCatch& try_catch,
                                    bool reportError,
                                    bool assertOnError) {
        bool haveError = false;

        if (try_catch.HasCaught() && try_catch.CanContinue()) {
            // normal JS exception
            _error = v8ExceptionToSTLString(&try_catch);
            haveError = true;
        }
        else if (hasOutOfMemoryException()) {
            // out of memory exception (treated as terminal)
            _error = "JavaScript execution failed -- v8 is out of memory";
            haveError = true;
        }
        else if (resultHandle.IsEmpty() || try_catch.HasCaught()) {
            // terminal exception (due to empty handle, termination, etc.)
            _error = "JavaScript execution failed";
            haveError = true;
        }

        if (haveError) {
            if (reportError)
                mongo::log() << _error << std::endl;
            if (assertOnError)
                mongo::uasserted(16722, _error);
            return true;
        }

        return false;
    }

} //not_mongo
