

//TODO: handle debug versions of node!
var pipeline = require(__dirname + '/../build/Release/pipeline.node');

console.log(pipeline.hello([{"$limit":1}], [{"hello":"world!"}])); // 'world'
