
var program = require('./')()
  , minimist = require('minimist')
  , result

result = program
  .register('abcd', function(args) {
    console.log('just do', args)
  })
  .register('args', function(args) {
    args = minimist(args)
    console.log('just do', args)
  })
  .register('abcde code', function(args) {
    console.log('doing something', args)
  })
  .register('another command', function(args) {
    console.log('anothering', args)
  })
  .parse(process.argv.splice(2))

if (result) {
  console.log('no command called, args', result)
}
