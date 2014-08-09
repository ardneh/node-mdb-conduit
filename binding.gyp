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
		"target_name": "copy_mongo_files",
      'type': 'none',
      'dependencies': [], # 'clone_mongo_src'
		"variables": {
			"mongo_src_dir": "src/third-party/mongo/src/mongo",
			"mongo_dest_dir": "<(LIB_DIR)/mongo",
		},
      'copies': [
			# Note: a ton of these just came from lite_parsed_query.*
			{
				'destination': '<(mongo_dest_dir)',
				'files': [
					"<(mongo_src_dir)/base",
					'<(mongo_src_dir)/bson',
					'<(mongo_src_dir)/pch.h',
					'<(mongo_src_dir)/pch.cpp',
					'<(mongo_src_dir)/platform',
					'<(mongo_src_dir)/logger',			# Do we really need all of this?
					],
			},
			{
				'destination': '<(mongo_dest_dir)/db',
				'files': [
						'<(mongo_src_dir)/db/pipeline',
						'<(mongo_src_dir)/db/server_options.h',
						'<(mongo_src_dir)/db/server_options.h',
					],
			},
			{
				"destination": "<(mongo_dest_dir)/util",
				"files": [
						"<(mongo_src_dir)/util/intrusive_counter.h",
						"<(mongo_src_dir)/util/intrusive_counter.cpp",
						"<(mongo_src_dir)/util/exit_code.h",
						"<(mongo_src_dir)/util/assert_util.h",
						"<(mongo_src_dir)/util/debug_util.h",
						"<(mongo_src_dir)/util/debug_util.cpp",
						"<(mongo_src_dir)/util/goodies.h",
						"<(mongo_src_dir)/util/time_support.h",
						"<(mongo_src_dir)/util/time_support.cpp",
						"<(mongo_src_dir)/util/allocator.h",
						"<(mongo_src_dir)/util/log.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.cpp",
						"<(mongo_src_dir)/util/heapcheck.h",
						"<(mongo_src_dir)/util/hex.h",
						"<(mongo_src_dir)/util/hex.cpp",
						"<(mongo_src_dir)/util/bufreader.h",
					]
			},
			{
				"destination": "<(mongo_dest_dir)/util/concurrency",
				"files": [
						"<(mongo_src_dir)/util/concurrency/thread_name.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.cpp",
						"<(mongo_src_dir)/util/concurrency/mutex.h",
						"<(mongo_src_dir)/util/concurrency/threadlocal.h",
						"<(mongo_src_dir)/util/concurrency/ticketholder.h",
					]
			},
			{
				"destination": "<(mongo_dest_dir)/util/net",
				"files": [
						"<(mongo_src_dir)/util/net/hostandport.h",
						"<(mongo_src_dir)/util/net/message.h",
						"<(mongo_src_dir)/util/net/message.cpp",
						"<(mongo_src_dir)/util/net/listen.h",
						"<(mongo_src_dir)/util/net/sock.h",
						"<(mongo_src_dir)/util/net/message_port.h",
						"<(mongo_src_dir)/util/net/message_port.cpp",
					]
			},
			{
				"destination": "<(mongo_dest_dir)/util/mongoutils",
				"files": [
						"<(mongo_src_dir)/util/mongoutils/str.h",
					]
			},
			{
				"destination": "<(mongo_dest_dir)/client",
				"files": [
						'<(mongo_src_dir)/client/constants.h',
						'<(mongo_src_dir)/client/undef_macros.h',
						'<(mongo_src_dir)/client/redef_macros.h',
						'<(mongo_src_dir)/client/export_macros.h',
					]
			},
			{
				"destination": "<(mongo_dest_dir)/base",
				"files": [
						'<(mongo_src_dir)/../../build/linux2/normal/mongo/base/error_codes.h',		# TODO: WE NED TO GENERATE THIS.
						'<(mongo_src_dir)/../../build/linux2/normal/mongo/base/error_codes.cpp',
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db",
				"files": [
						"<(mongo_src_dir)/db/jsobj.h",
						"<(mongo_src_dir)/db/jsobj.cpp",
						"<(mongo_src_dir)/db/dbmessage.h",
						"<(mongo_src_dir)/db/dbmessage.cpp",
					]
			},
			{
				# Part of mongoscore lib and used by pipeline.cpp.
				"destination": "<(mongo_dest_dir)/db/query",
				"files": [
						"<(mongo_src_dir)/db/query/lite_parsed_query.cpp",
						"<(mongo_src_dir)/db/query/lite_parsed_query.h",
					]
			}
		]},{
			# See https://code.google.com/p/gyp/wiki/GypUserDocumentation#Skeleton_of_a_typical_library_target_in_a_.gyp_file
			"target_name": "pipeline",
			'type': '<(library)',
			'dependencies': ['copy_mongo_files'],
			"variables": {
				"mongo_dest_dir": "<(LIB_DIR)/mongo",
				"boost_dir": "src/third-party/mongo/src/third_party/boost",
			},
			"sources": [
				'<(mongo_dest_dir)/pch.cpp',
				'!@(ls -1 <(mongo_dest_dir)/bson/*.cpp)',
				'!@(ls -1 <(mongo_dest_dir)/db/pipeline/*.cpp)',
				'!@(ls -1 <(mongo_dest_dir)/base/*.cpp)',
				'<(mongo_dest_dir)/util/intrusive_counter.cpp',
				'<(mongo_dest_dir)/db/query/lite_parsed_query.cpp',
			],
			"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
			"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
			# TEMP TODO: get the compiler flags by using mongo's buildinfo.cpp.
			# TEMP TODO: make sure we are using the same allocator.
			"cflags": [
				"-Wno-ignored-qualifiers", "-Wno-extra",
				"-Wnon-virtual-dtor", "-Woverloaded-virtual", "-fPIC", "-fno-strict-aliasing", "-ggdb", "-pthread", "-Wall", "-Wsign-compare", "-Wno-unknown-pragmas", "-Winvalid-pch", "-pipe", "-Werror", "-O3", "-Wno-unused-local-typedefs", "-Wno-unused-function", "-Wno-deprecated-declarations", "-fno-builtin-memcmp"
			],
			"defines!": [ "_DEBUG" ],  # Prevent mutexDebugger from being included.  Debug build of mongo doesn't seem to include it.
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
				"<(LIB_DIR)",
				"<(mongo_dest_dir)",
				"<(boost_dir)",
			],
		},{
		"target_name": "mungedb-aggregate-native",
		'dependencies': ['pipeline'],
		"sources": [
			"src/Pipeline.cpp",
			"src/mongo-ours/db/interrupt_status_noop.cpp",
			"src/mongo-ours/db/pipeline/document_source_v8.cpp",
			"src/MongoV8Helpers.cpp",
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		# TEMP TODO: get the compiler flags by using mongo's buildinfo.cpp.
		# TEMP TODO: make sure we are using the same allocator.
		"cflags": [
			"-Wno-ignored-qualifiers", "-Wno-extra",
			"-Wnon-virtual-dtor", "-Woverloaded-virtual", "-fPIC", "-fno-strict-aliasing", "-ggdb", "-pthread", "-Wall", "-Wsign-compare", "-Wno-unknown-pragmas", "-Winvalid-pch", "-pipe", "-Werror", "-O3", "-Wno-unused-local-typedefs", "-Wno-unused-function", "-Wno-deprecated-declarations", "-fno-builtin-memcmp"
		],
		"defines!": [ "_DEBUG" ],  # Prevent mutexDebugger from being included.  Debug build of mongo doesn't seem to include it.
		"variables": {
			"our_dir": "<(module_root_dir)", # TODO: use a pre-defined variable.  It doesn't look like gyp provides one?
			"third_party_dir": "<(our_dir)/src/third-party",
			"mongo_dir": "<(third_party_dir)/mongo",
			"mongo_build_type": "normal",
			#"mongo_build_type": "d",
			"mongo_build_name": "<!(python bin/get_mongo_platform.py)",
			"mongo_build_dir": "<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)",
		},
		"copies__": [
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
			"<(mongo_dir)/src/mongo/",  # The pipeline files do include "bson/"
			"<(mongo_build_dir)"
		],
		"libraries": [
			"-lpipeline",
		],
		"libraries__": [
			"-lbson",
			"-lbase",				# Needed for initializers that are used to add all of the expressions at startup.
			"-lexpressions",		# $match.  mongo/db/matcher/expression_parser.cpp, and probably mongo/db/matcher/expression_parser_tree.cpp
			"-lpcrecpp",			# $match via expressions parser.
			"-lpath",				# dyn failure after adding $match stuff.
			"-lcommon",				# FieldRef::FieldRef dynamic lookup.  Not sure if $match needs this or not.
			"-lcoredb",			# All of the pipeline stuff.
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
			"-lcoreserver",
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
