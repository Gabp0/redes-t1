# Redes - Trabalho III - Janela Deslizante

## Autores:
* Gabriel de Oliveira Pontarolo - GRR20203895
* Rodrigo Saviam Soffner - GRR20205092

## Compilação:
* O arquivo **Makefile** faz a compilação a partir do comando *make*, além de incluir funções *purge* e *purge* para limpeza, gerando o executável **socket**.
* 5% de frames perdido default, possível mudar no githyanki.h, chanceToReceive const.

## Execução:
* O executável precisa ser executado sendo SU, utiliza duas interfaces de rede loopback criadas, chamadas lo1 e lo2. Pode mudar para apenas uma interface no main, porém não recomendado.
* Para o funcionamento do programa, abrir em dois terminais um utilizando '-s', para o server e o outro '-r' para o client.
* Saídas, dentro da pasta logs, clientLog e serverLog, logs dos programas e buffer o texto que foi enviado. Arquivo text com o texto a ser enviado.
* Disclaimer: A string que será transmitida está contida dentro do main.c.


## Sobre:
* Implementação da Janela Deslizante Seletiva, dentro do T1 de redes (T1 se encontra em uma versão não final).
* Realizando a execução correta do programa, resulta no envio de um texto, o texto pode ser encontrado em /logs/text, do server para o client, utilizado os métodos do T1 e a janela deslizante implementada.