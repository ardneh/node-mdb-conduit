{
	"targets": [{
		"target_name": "pipeline",
		"sources": [
			"src/Pipeline.cpp"
		],
		"cflags!": [ "-fno-exceptions" ],
		"cflags_cc!": [ "-fno-exceptions" ],
		"conditions": [
		["OS=='mac'", {
			"xcode_settings": {
				"GCC_ENABLE_CPP_EXCEPTIONS": "YES"
			}
		}]],
		"include_dirs": [
			"/Users/cezell/src/mongo_next/src/src/third_party",
			"/Users/cezell/src/mongo_next/src",
			"/Users/cezell/src/mongo_next/build/darwin/normal"
		],
		"libraries": [
			"-lcoredb", "-L/Users/cezell/src/mongo_next/build/darwin/normal/mongo"
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
