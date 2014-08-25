{
	"targets": [
		{
			"target_name": "mdb-conduit",
			"dependencies": [
				"third_party/mdb-conduit/mdb-conduit.gyp:libmdb-conduit",
			],
			"include_dirs": [
				"third_party",
				"src",
				"<(LIB_DIR)",
			],
			"ldflags": [
				"-L<(LIB_DIR)/..",
			],
			"sources": [
				"src/module.cpp",
				"src/db/pipeline/document_source_v8.cpp",
				"src/MongoV8Helpers.cpp",
			],
		}
	]  # End targets.
}
