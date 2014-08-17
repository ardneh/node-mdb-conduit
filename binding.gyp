{
	"targets": [
		{
			"target_name": "mdb-conduit",
			"dependencies": [
				"mdb-conduit.gyp:libmdb-conduit",
			],
			"include_dirs": [
				"src",
				"<(LIB_DIR)",
			],
			"libraries": [
				"-lmdb-conduit",
				"-L<(LIB_DIR)",
			],
			"sources": [
				"src/module.cpp",
				"src/db/pipeline/document_source_v8.cpp",
				"src/MongoV8Helpers.cpp",
			],
		}
	]  # End targets.
}
