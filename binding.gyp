{
	"targets": [
		{
			"target_name": "mdb-conduit",
			"dependencies": [
				"third_party/mdb-conduit/mdb-conduit.gyp:libmdb-conduit",
			],
			"include_dirs": [
				"third_party/mdb-conduit/src",
				"src",
				"<(LIB_DIR)",
			],
			"libraries": [
				"-L<(LIB_DIR)/..",
				"-lmdb-conduit",
			],
			"sources": [
				"src/module.cpp",
				"src/db/pipeline/document_source_v8.cpp",
				"src/MongoV8Helpers.cpp",
			],
		}
	]  # End targets.
}
