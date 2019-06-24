#!/bin/bash

# Verifica se foi passado um path do programa a ser compilado
if [[ ! -z $1 ]] ; then
    # Caminho do programa a ser compilado
    PROG_PATH=$(realpath -q $1)
fi

# Compila o código
flex cminus.l
bison -d cminus.y
# C_FLAGS - Variável para adicionar flags do ambiente na compilação (TravisCI)
gcc -c $C_FLAGS *.c
gcc -o cminus *.o -ly -lfl

# Executa a compilação
./cminus ${PROG_PATH} $2

# Limpa o ambiente após a compilação (Remove arquivos gerados)
rm -rf *.tab.*
rm -rf *.yy.*
rm -rf *.gch
rm -rf *.o
rm cminus

