## Branch que contém o backend do compilador online.

O backend do compilador está hospedado em uma _lambda function_ na AWS.

Por conta de limitações do ambiente, foi necessário fazer o _upload_ do compilador já em formato binário. Com isso, foi possível reduzir consideravelmente o tamanho do pacote implantado e também o tempo de resposta, uma vez que não é necessário compilar o código do compilador. Esta medida foi tomada visando diminuir ao máximo os custos de execução desta função.

Para obter o binário do compilador para execução na _lambda function_, foi necessário compilá-lo em uma instancia EC2 com uma imagem Linux Amazon AMI. Passos detalhados desse processo são mostrados no link: https://aws.amazon.com/pt/premiumsupport/knowledge-center/lambda-linux-binary-package/
