{
  "targets": [
    {
      "target_name": "pipeline",
      "sources": [
	  		"src/Pipeline.cpp"
		]
    }
  ],
  'conditions': [
        ['OS=="mac"', {
          'include_dirs': [
            '/Users/cezell/src/mongo_next/src/mongo/db/pipeline'
          ],
          'libraries': [
          ],
        }]
	],
	'foo': [
    		'/Users/cezell/src/mongo_next/build/darwin/normal/mongo/libcoredb.a'
	]
}
