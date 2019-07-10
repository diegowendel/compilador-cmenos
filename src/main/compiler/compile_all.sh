#!/bin/bash

# Compila o código
flex cminus.l
bison -d cminus.y
# C_FLAGS - Variável para adicionar flags do ambiente na compilação (TravisCI)
gcc -c $C_FLAGS *.c
gcc -o cminus *.o -ly -lfl

COMPILER_PATH=$(pwd)

for file in ../programs/*.c; do
    PROG_PATH=$(realpath -q ${file})
    ${COMPILER_PATH}/cminus ${PROG_PATH}
done

# Compila bios
${COMPILER_PATH}/cminus $(realpath -q ../os/bios.c) -b

# Compila kernel
${COMPILER_PATH}/cminus $(realpath -q ../os/kernel.c) -k

# Limpa o ambiente após a compilação (Remove arquivos gerados)
rm -rf *.tab.*
rm -rf *.yy.*
rm -rf *.gch
rm -rf *.o
rm cminus

echo -e "\nAll files compiled."

