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
		"target_name": "pipeline",
		"sources": [
			"src/Pipeline.cpp",
			"src/mongo-ours/db/interrupt_status_noop.cpp",
			"src/MongoV8Helpers.cpp",

			# TODO: make libs for these.
			"src/mongo/util/intrusive_counter.cpp",
			"src/mongo/db/query/lite_parsed_query.cpp"
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		"defines!": [ "_DEBUG" ],  # Prevent mutexDebugger from being included...  Debug build of mongo doesn't seem to include it.
		"variables": {
			"mongo_dir": "/Users/cezell/src/mongo_next",
			"mongo_build_type": "normal"
		},
		"copies": [
			{
				"destination": "src/mongo/util",
				"files": [
					"<(mongo_dir)/src/mongo/util/intrusive_counter.cpp"
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
				"variables": {
					"mongo_build_name": "darwin",
				},
				"xcode_settings": {
					"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
					"GCC_ENABLE_CPP_RTTI": "YES"
				}
			}]
		],
		"include_dirs": [
			"src/third-party",
			"<(mongo_dir)/src/third_party",
			"<(mongo_dir)/src",
			"<(mongo_dir)/src/mongo/",  # The pipeline files do include "bson/..."
			"<(mongo_dir)/build/<(mongo_build_name)/>(mongo_build_type)"
		],
		"libraries": [
			"-lbson",
			"-lcoredb",
			"-lfoundation",
			"-lstringutils",
			"-lplatform",
			"-lbase",
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
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/base",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/platform",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/logger",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/db",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/util",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/util/concurrency",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/third_party/boost",

			# TODO: Get rid of this!  should not need any auth.
			"-lauthcore",
			"-L<(mongo_dir)/build/<(mongo_build_name)/<(mongo_build_type)/mongo/db/auth"
		]
	}]
}
