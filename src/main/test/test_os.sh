#!/bin/bash

# declare -r -> Declara uma variável read-only
# Colors
declare -r PRETO=$(tput setaf 0)
declare -r VERMELHO=$(tput setaf 1)
declare -r VERDE=$(tput setaf 2)
declare -r AMARELO=$(tput setaf 3)
declare -r AZUL=$(tput setaf 4)
declare -r ROXO=$(tput setaf 5)
declare -r CIANO=$(tput setaf 6)
declare -r CINZA=$(tput setaf 7)
declare -r BRANCO=$(tput setaf 8)
declare -r BOLD=$(tput bold)
declare -r NORMAL=$(tput sgr0)
declare -r UNDERLINE=$(tput smul)

declare -r VERMELHO_BK=$(tput setab 1)
declare -r VERDE_BK=$(tput setab 2)
declare -r AMARELO_BK=$(tput setab 3)

# Declare integers
declare -i COUNT_CORRECTS=0
declare -i COUNT_INCORRECTS=0

echo "Verifica se houve alterações nos programas compilados."
echo -e "Script utilizado como apoio para refatorações (testes de regressão)\n"

echo -e "${BOLD}os tests...${NORMAL}\n"

COUNT_OS=`ls -1 ../os/*.txt 2>/dev/null | wc -l`
if [ $COUNT_OS -ne 0 ]; then
    for file in ../os/*.txt; do
		# Programa compilado
		PROG_PATH=$(realpath -q ${file})
		# Nome base do aquivo (sem path)
		BASENAME=$(basename ${file})
		# Arquivo com resultado esperado
		EXPECTED_OS=expected_os/${BASENAME}

		echo "${BOLD}- ${BASENAME}${NORMAL}"

		# Verifica se os arquivos são iguais
		diff -q ${PROG_PATH} ${EXPECTED_OS}

		# Variável que indica erro na execução do último comando
		result=$?

		# Printa sucesso ou falha no diff
		if [[ $result -ne 0 ]]; then
		    echo -e "\t${VERMELHO}${BOLD}Fail${NORMAL}"
		    ((COUNT_INCORRECTS+=1))
		else
		    echo -e "\t${VERDE}${BOLD}Success${NORMAL}"
		    ((COUNT_CORRECTS+=1))
		fi
	done

	echo -e "\n${VERDE}${BOLD}${COUNT_CORRECTS}${NORMAL} corrects and ${VERMELHO}${BOLD}${COUNT_INCORRECTS}${NORMAL} incorrects.\n"
else
    echo -e "${VERMELHO}${BOLD}Não há arquivos compilados na pasta ${UNDERLINE}os${NORMAL}\n"
    exit 1
fi

if [ $COUNT_INCORRECTS -ne 0 ]; then
    echo -e "${VERMELHO}${BOLD}Foram encontrados ${COUNT_INCORRECTS} erros.${NORMAL}\n"
    exit 1
fi

