{
	"targets": [{
		"target_name": "pipeline",
		"sources": [
			"src/Pipeline.cpp",
			"src/MongoV8Helpers.cpp"
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		"conditions": [
			["OS=='mac'", {
				"xcode_settings": {
					"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
					"GCC_ENABLE_CPP_RTTI": "YES"
				}
			}]
		],
		"include_dirs": [
			"/Users/cezell/src/mongo_next/src/src/third_party",
			"/Users/cezell/src/mongo_next/src",
			"/Users/cezell/src/mongo_next/build/darwin/normal"
		],
		"libraries": [
			"-lbson",
			"-lcoredb",
			"-lfoundation",
			"-lstringutils",
			"-lplatform",
			"-lbase",
			"-llogger",
			"-lboost_system",
			"-lboost_thread",
			"-L/Users/cezell/src/mongo_next/build/darwin/normal/mongo",
			"-L/Users/cezell/src/mongo_next/build/darwin/normal/mongo/base",
			"-L/Users/cezell/src/mongo_next/build/darwin/normal/mongo/platform",
			"-L/Users/cezell/src/mongo_next/build/darwin/normal/mongo/logger",
			"-L/Users/cezell/src/mongo_next/build/darwin/normal/third_party/boost"
		]
	}],
	"conditions____foo": [
		["OS=='mac'", {
			"include_dirs": [
				"/Users/cezell/src/mongo_next/src/src/third_party",
				"/Users/cezell/src/mongo_next/src/"
			],
			"libraries": [
				"/Users/cezell/src/mongo_next/build/darwin/normal/mongo/libcoredb.a"
				]
		},
		{
			"variables": {
				"MONGO_SRC_PATH": ""
		}}]
	]
}
