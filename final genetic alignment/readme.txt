=====================================================================
==============PROJETO FINAL BIOLOGIA COMPUTACIONAL===================
=====================================================================
Pasos para compilar o codigo:

1.- Primero, se deve compilar o codigo com o nome start_alignment.cpp

g++ star_alignment.cpp -o a

2.- Segundo, compilar ./a e verificar se a base de dados referenciada 
é a correta, sao 10 arquivos na base de dados com o nome de input(numero).txt, e verificar se os arquivos start_output.txt e start_output.txt existem
 Se precisa fazer compilar a seguinte linha: 

./a < dataset/input1.txt

3.- Terceiro, compilar o codigo com o nome genetic_alignment.cpp

g++ genetic_alignment.cpp -o a

4.- Quarto, compilar a seguiente linha de codigo:

./a < star_output_input.txt

5.- Quinto, se pode mudar as configuracoes dos parametros para o algoritmo genetico, por defeito esta com uma populacao de 200, e 1000 geracoes.


