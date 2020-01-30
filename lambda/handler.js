'use strict';

const cmd = require('node-cmd');
const fs = require('fs');
const promise = require('bluebird');

module.exports.compile = async event => {
  const params = event.body;
  const paramsJSON = JSON.parse(params);
  const code = paramsJSON.code;
  const getAsync = promise.promisify(cmd.get, { multiArgs: true, context: cmd });

  /**
   * Compila o código recebido e armazena o resultado em um arquivo temporário.
   *
   * Obs: Para escrever no filesystem usando lambda functions é obrigatório utilizar o diretório /tmp
   */
  const commandsResult = await getAsync(`
    touch /tmp/temp.c
    echo '${code}' > /tmp/temp.c
    cp ./compiler/cminus /tmp/cminus
    /tmp/cminus /tmp/temp.c
    ls /tmp > /tmp/ls_temp.txt
  `);

  let compiledCode;

  try {
    compiledCode = fs.readFileSync('/tmp/temp.txt').toString();
    console.log('Compilation Success!');
  } catch(err) {
    console.error('Compilation Error:', err);
    const commandsErrorArray = commandsResult;
    compiledCode = `Compilation Error!
    ${commandsErrorArray[0]}`;
  }

  /**
   * Limpa os arquivos temporários após a compilação.
   */
  await getAsync('rm /tmp/*');

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
