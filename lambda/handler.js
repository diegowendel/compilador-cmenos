'use strict';

const cmd = require('node-cmd');
const fs = require('fs');
const promise = require('bluebird');

module.exports.compile = async event => {
  const params = event.body;
  const paramsJSON = JSON.parse(params);
  const code = paramsJSON.code;
  console.log('code:', code);

  const getAsync = promise.promisify(cmd.get, { multiArgs: true, context: cmd });

  // Para escrever no filesystem usando lambda functions é obrigatório utilizar o diretório /tmp
  const commands = await getAsync(`
    touch /tmp/temp.c
    echo '${code}' > /tmp/temp.c
    cp ./compiler/cminus /tmp/cminus
    /tmp/cminus /tmp/temp.c
    ls /tmp > /tmp/ls_temp.txt
  `);

  // TODO: handle error cases

  console.log('commands', commands);
  const lsTemp = fs.readFileSync('/tmp/ls_temp.txt').toString();
  console.log('lsTemp', lsTemp);
  const compiledCode = fs.readFileSync('/tmp/temp.txt').toString();

  const response = {
    statusCode: 200,
    headers: {
      'Access-Control-Allow-Origin': '*',
      'Access-Control-Allow-Credentials': true,
    },
    body: JSON.stringify({
      message: compiledCode,
      input: event,
    }),
  };

  return response;
};
