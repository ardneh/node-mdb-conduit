

//TODO: handle debug versions of node!
var pipeline = require(__dirname + '/../build/Debug/pipeline.node');
//var pipeline = require(__dirname + '/../build/Release/pipeline.node');

function testBSONConversion(data) {
	console.log(data, "=?", pipeline.hello(data, []));
}

//testBSONConversion([{'v':132},{'v':377},{'v':358},{'v':108},{'v':28},{'v':741},{'v':68}]);
//testBSONConversion([42]);
//testBSONConversion([1,2,3,{'a':'b'}]);

//The future:
console.log(pipeline.hello([{"$limit":1}], [{"hello":"world!"}, {"second":"obj"}]));
