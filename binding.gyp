# Here is an incomplete list of things that we need to make a mongo build to get:
# * mongo/base/error_codes.h

{
	"includes": [
		"mongo_src_list.gypi",
	],
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
		"target_name": "mungedb-aggregate-native",
		"variables": {
			"mongo_src_dir": "src/third-party/mongo/src/mongo",
			"third_party_src_dir": "src/third-party/mongo/src/third_party",
			"third_party_dest_dir": "<(LIB_DIR)/third_party",
			# GRRR.
			"mongo_dest_dir": "build/Release/obj.target/mongo", # "<(LIB_DIR)/mongo",
			"boost_dir": "src/third-party/mongo/src/third_party/boost",
			"our_dir": "<(module_root_dir)", # TODO: use
		},
		"include_dirs": [
			"src",
			"<(LIB_DIR)",
			"<(boost_dir)",
			"<(mongo_dest_dir)",
		],
		"sources": [
			# Our stuff.
			"src/Pipeline.cpp",
			"src/mongo-ours/db/interrupt_status_noop.cpp",
			"src/mongo-ours/db/pipeline/document_source_v8.cpp",
			"src/MongoV8Helpers.cpp",

			# Mongo's stuff.
			'<@(mongo_src_files)',
			#'<!@(bash -c "find build/Release/obj.target/mongo -name \"*.cpp\" || true")',
			#'<(mongo_dest_dir)/pch.cpp',
			#'>!@(ls -1m build/Release/obj.target/mongo/bson/*.cpp',
			#">!@(ls -1 >(mongo_dest_dir)/db/pipeline/*.cpp)",
			#'>!@(ls -1 >(mongo_dest_dir)/base/*.cpp)',
			#'<(mongo_dest_dir)/util/intrusive_counter.cpp',
			#'<(mongo_dest_dir)/db/query/lite_parsed_query.cpp',
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		# TEMP TODO: get the compiler flags by using mongo's buildinfo.cpp.
		# TEMP TODO: make sure we are using the same allocator.
		"cflags": [
			"-Wno-ignored-qualifiers", "-Wno-extra",
			"-Wnon-virtual-dtor", "-Woverloaded-virtual", "-fPIC", "-fno-strict-aliasing", "-ggdb", "-pthread", "-Wall", "-Wsign-compare", "-Wno-unknown-pragmas", "-Winvalid-pch", "-pipe", "-Werror", "-O3", "-Wno-unused-local-typedefs", "-Wno-unused-function", "-Wno-deprecated-declarations", "-fno-builtin-memcmp"
		],
		"defines": [ "MONGO_EXPOSE_MACROS=1" ],
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

						# Compile.
						"<(mongo_src_dir)/util/timer.h",
						"<(mongo_src_dir)/util/timer-inl.h",
						"<(mongo_src_dir)/util/timer-generic-inl.h",
						"<(mongo_src_dir)/util/background.h",
						"<(mongo_src_dir)/util/string_map.h",
						"<(mongo_src_dir)/util/unordered_fast_key_table.h",
						"<(mongo_src_dir)/util/unordered_fast_key_table_internal.h",
						"<(mongo_src_dir)/util/text.h",
						"<(mongo_src_dir)/util/base64.h",
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

						# compile.
						'<(mongo_src_dir)/client/connpool.h',
						'<(mongo_src_dir)/client/dbclientinterface.h',
						'<(mongo_src_dir)/client/dbclientcursor.h',
						'<(mongo_src_dir)/client/syncclusterconnection.h',
						'<(mongo_src_dir)/client/dbclient_rs.h',
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
				"destination": "<(mongo_dest_dir)/s",
				"files": [
						# Compile.
						'<(mongo_src_dir)/s/collection_metadata.h',
						'<(mongo_src_dir)/s/chunk_version.h',
						'<(mongo_src_dir)/s/bson_serializable.h',
						'<(mongo_src_dir)/s/range_arithmetic.h',
						'<(mongo_src_dir)/s/type_chunk.h',
						'<(mongo_src_dir)/s/shard.h',
						'<(mongo_src_dir)/s/strategy.h',
						'<(mongo_src_dir)/s/chunk.h',
						'<(mongo_src_dir)/s/distlock.h',
						'<(mongo_src_dir)/s/shardkey.h',
						'<(mongo_src_dir)/s/shard_key_pattern.h',
						'<(mongo_src_dir)/s/request.h',
						'<(mongo_src_dir)/s/config.h',
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db",
				"files": [
						# include.
						"<(mongo_src_dir)/db/jsobj.h",
						"<(mongo_src_dir)/db/jsobj.cpp",
						"<(mongo_src_dir)/db/dbmessage.h",
						"<(mongo_src_dir)/db/dbmessage.cpp",

						# compile.
						"<(mongo_src_dir)/db/interrupt_status.h",
						"<(mongo_src_dir)/db/namespace_string.h",
						"<(mongo_src_dir)/db/namespace_string-inl.h",
						"<(mongo_src_dir)/db/clientcursor.h",
						"<(mongo_src_dir)/db/diskloc.h",
						"<(mongo_src_dir)/db/keypattern.h",
						"<(mongo_src_dir)/db/field_ref.h",
						"<(mongo_src_dir)/db/matcher",
						"<(mongo_src_dir)/db/invalidation_type.h",
						"<(mongo_src_dir)/db/field_ref_set.h",
						"<(mongo_src_dir)/db/matcher.h",
						"<(mongo_src_dir)/db/projection.h",
						"<(mongo_src_dir)/db/sorter",
						"<(mongo_src_dir)/db/json.h",
					]
			},
			{
				# Part of mongoscore lib and used by pipeline.cpp.
				"destination": "<(mongo_dest_dir)/db/query",
				"files": [
						# include.
						"<(mongo_src_dir)/db/query/lite_parsed_query.cpp",
						"<(mongo_src_dir)/db/query/lite_parsed_query.h",

						# Compile
						"<(mongo_src_dir)/db/query/runner.h",
						"<(mongo_src_dir)/db/query/canonical_query.h",
						"<(mongo_src_dir)/db/query/parsed_projection.h",
					]
			},
			{
				"destination": "<(mongo_dest_dir)/scripting",
				"files": [
						"<(mongo_src_dir)/scripting/v8_utils.h",
					]
			},
			{
				"destination": "<(third_party_dest_dir)",
				"files": [
						# Compile
						"<(third_party_src_dir)/murmurhash3",
					]
			},
		]
	}]  # End targets.
}
