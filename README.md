# Redes - Trabalho III - Janela Deslizante

## Autores:
* Gabriel de Oliveira Pontarolo - GRR20203895
* Rodrigo Saviam Soffner - GRR20205092

## Compilação:
* O arquivo **Makefile** faz a compilação a partir do comando *make*, além de incluir funções *clean* e *purge* para limpeza, gerando o executável **chat**.

## Execução:
* **./chat {interface 1} {interface 2}**, precisa ser executado com permissão de administrador.
* Pode ser executado em uma única maquina com dois terminais e uma interface para envio e outra para recebimento
  * i.e. **./chat lo1 lo2** em um terminal e **./chat lo2 lo1** em outro
* Pode ser executado entre duas máquinas conectadas por um cabo de rede, sendo interface 1 e 2 a mesma.
  * i.e. **./chat enps01 enps01** em uma e **./chat eno1 eno1** na outra
  
## Comandos do chat:
* **/send {caminho do arquivo} {nome para recebimento}** faz o envio de um arquivo de mídia para a outra máquina.
* **/quit** ou **/q** fecha a aplicação.

## Códigos
* **200** Sucesso
* **202** Sem conexão
* **203** Comando inválido
* **204** Falha *(não implementado)*
* **205** Arquivo não encontrado
* **206** Falha ao abrir o arquivo
* **207** Falha ao escrever o arquivo no destino *(não implementado)*
* **208** Conexão restaurada *(não implementado)*