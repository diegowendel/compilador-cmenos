const cmd = require('node-cmd');
const fs = require('fs');
const promise = require('bluebird');

const readFile = (file) => {
  return fs.readFileSync(file, 'utf8');
}

module.exports = (app) => {
  const CompilerController = {
    compile(req, res) {
      const code = String(req.body.code);
      const getAsync = promise.promisify(cmd.get, { multiArgs: true, context: cmd });
      getAsync(`
        cd ../src/main/programs
        echo '${code}' > temp.c
        cd ../compiler
        ./compila.sh ../programs/temp.c
      `).then(data => {
        const compiledCode = readFile('../src/main/programs/temp.txt');
        res.status(200).json({'code': compiledCode});
      }).catch(err => {
        console.log('cmd err', err);
      });
    }
  };

  return CompilerController;
};
