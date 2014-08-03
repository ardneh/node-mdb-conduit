# Here is an incomplete list of things that we need to make a mongo build to get:
# * mongo/base/error_codes.h

{
	"target_defaults": {
		"configurations": {
			"Debug": {  # TODO: figure out how to get this working!
				"variables": {
					"mongo_build_type": "d"
				}
			},
			"Release": {
				"variables": {
					"mongo_build_type": "normal"
				}
			}
		},
	},
	"targets": [{
	#	"target_name": "clone_mongo",	# TODO: clone mongo into src/third-party.  The the pre-built option in node-gyp might provide a good example.
						# See: https://code.google.com/p/gyp/wiki/GypLanguageSpecification#Actions
						# And maybe: http://stackoverflow.com/questions/18102858/how-to-move-gyp-target-to-a-separate-include-file
	#	"git clone -b v2.6 --depth 2 --single-branch https://github.com/mongodb/mongo src/third-party/mongo"
	#}, {
	#	"target_name": "clone_mongo_cxx_driver", # TODO: for the json->bson parser :)  Assumes it will not be in the mongo src for much longer.
	#}, {
		"target_name": "pipeline",
		"sources": [
			"src/Pipeline.cpp",
			"src/mongo-ours/db/interrupt_status_noop.cpp",
			"src/mongo-ours/db/pipeline/document_source_v8.cpp",
			"src/MongoV8Helpers.cpp",
			#"src/third-party/js-bson/ext/bson.cc",

			# The 'default' intitalizer function looks to be getting optimized out of the build
			# causing initialization to fail.  Including this here to try and prevent that from happening.
			"src/mongo/base/init.cpp",

			# TODO: make libs for these.
			"src/mongo/util/intrusive_counter.cpp",
			"src/mongo/db/query/lite_parsed_query.cpp"
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		# TEMP TODO: get the compiler flags by using mongo's buildinfo.cpp.
		# TEMP TODO: make sure we are using the same allocator.
		"cflags": [
			"-Wno-ignored-qualifiers", "-Wno-extra",
			"-Wnon-virtual-dtor", "-Woverloaded-virtual", "-fPIC", "-fno-strict-aliasing", "-ggdb", "-pthread", "-Wall", "-Wsign-compare", "-Wno-unknown-pragmas", "-Winvalid-pch", "-pipe", "-Werror", "-O3", "-Wno-unused-local-typedefs", "-Wno-unused-function", "-Wno-deprecated-declarations", "-fno-builtin-memcmp"
		],
		"defines!": [ "_DEBUG" ],  # Prevent mutexDebugger from being included...  Debug build of mongo doesn't seem to include it.
		"variables": {
			"our_dir": "<!(pwd)", # TODO: use a pre-defined variable.  It doesn't look like gyp provides one?
			"third_party_dir": "<(our_dir)/src/third-party",
			"mongo_dir": "<(third_party_dir)/mongo",
			"mongo_build_type": "normal",
			#"mongo_build_type": "d",
			"mongo_build_name": "<!(python get_mongo_platform.py)",
			"mongo_build_dir": "<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)",
		},
		"copies": [
			{
				"destination": "src/mongo/util",
				"files": [
					"<(mongo_dir)/src/mongo/util/intrusive_counter.cpp"
				]
			},
			{
				"destination": "src/mongo/base",
				"files": [
					"<(mongo_dir)/src/mongo/base/init.cpp"
				]
			},
			{
				# Part of mongoscore lib and used by pipeline.cpp.
				"destination": "src/mongo/db/query",
				"files": [
					"<(mongo_dir)/src/mongo/db/query/lite_parsed_query.cpp"
				]
			}

			# TODO: copy over the files in db/pipeline that we need (and provide a way to apply patches on them if necessary).
		],
		"conditions": [
			["OS=='mac'", {
				#"variables": {
				#	"mongo_build_name": "darwin",
				#},
				"xcode_settings": {
					"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
					"GCC_ENABLE_CPP_RTTI": "YES"
				}
			}],
			["OS=='linux'", {
				#"variables": {
				#	"mongo_build_name": "linux",
				#}
			}]
		],
		"include_dirs": [
			"src",
			"src/third-party",
			"<(mongo_dir)/src/third_party",
			"<(mongo_dir)/src/third_party/boost",
			"<(mongo_dir)/src",
			"<(mongo_dir)/src/mongo/",  # The pipeline files do include "bson/..."
			"<(mongo_build_dir)"
		],
		"libraries": [
			"-lbson",
			"-lbase",				# Needed for initializers that are used to add all of the expressions at startup.
			"-lexpressions",		# $match.  mongo/db/matcher/expression_parser.cpp, and probably mongo/db/matcher/expression_parser_tree.cpp
			"-lpcrecpp",			# $match via expressions parser.
			"-lpath",				# dyn failure after adding $match stuff.
			"-lcommon",				# FieldRef::FieldRef dynamic lookup.  Not sure if $match needs this or not.
			"-lcoredb",
			"-lfoundation",
			"-lstringutils",
			"-lplatform",
			"-llogger",
			"-lthread_name", # For logger.
			"-llasterror", # For logger.
			"-lstacktrace", # For logger.
			"-lserver_parameters",  # Who knows, really need to get rid of this one.
			"-lclientdriver",		# Another misc. dependency because of all the other libs I've pulled in.
			"-lbackground_job",		# Yay for everything!
			"-lserver_options_core",# Another misc.
			"-lspin_lock",# Another misc.
			"-lnetwork",# Another misc.
			"-lfail_point",# Required by lnetwork
			"-lboost_system",
			"-lboost_thread",
			"-lmurmurhash3", # Needed by -lbase and intializer stuff.
			"-L<(mongo_build_dir)/mongo",
			"-L<(mongo_build_dir)/mongo/base",
			"-L<(mongo_build_dir)/mongo/platform",
			"-L<(mongo_build_dir)/mongo/logger",
			"-L<(mongo_build_dir)/mongo/db",
			"-L<(mongo_build_dir)/mongo/util",
			"-L<(mongo_build_dir)/mongo/util/concurrency",
			"-L<(mongo_build_dir)/third_party/boost",
			"-L<(mongo_build_dir)/third_party/murmurhash3",
			"-L<(mongo_build_dir)/third_party/pcre-8.30",

			# TODO: Get rid of this!  should not need any auth.
			"-lauthcore",
			"-L<(mongo_build_dir)/mongo/db/auth"
		]
	}]
}
