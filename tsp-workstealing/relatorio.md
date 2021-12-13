## TSP com workstealing

### Como executar

O projeto tem um `Makefile`. Para compilar o programa basta executar `make build`.  
Para executar o código utilizar `make run`.   
Para alterar o número de _threads_ basta modificar a constante `NUM_THREADS` no arquivo `main.c`.  
Para alterar a instância utilizada no programa, basta alterar o nome do arquivo na constante `FILENAME` no arquivo `main.c`. As opções de instâncias estão na pasta `/instances`.   
Ao executar o código deve-se informar o número de cidades da instância escolhida.

### Workstealing
O Workstealing consiste em "roubar" da fila de outra thread uma _task_ caso a sua fila de _task_ esteja vazia. No cenário do TSP vamos enfileirando e processando na estrutura de cada uma das threads as rotas. Quando a fila de uma das threads fica vazia, então essa thread tenta "roubar" uma rota da fila de outra thread, e para isso funcionar o "roubo" acontece na outra ponta da fila (por isso a estrutura usada é uma DEque, double ended queue). 

### Desenvolvimento

Foi utilizado como base o trabalho anterior, onde foi desenvolvido o TSP com MPI e Threads. Para desenvolvimento do cenário atual, foram feitar modificações no código original em duas etapas. A primeira para garantir o funcionamento do TSP sem o MPI e com balanceamento de threads por divisão das stack (como era a proposta do trabalho anterior). A segunda com objetivo de modificar a estrutura de dados de stack para deque, e adicionar o workstealing em vez do stack split para balanceamento das threads.


#### Testes

Foram feitos testes para instância de 12 cidades e verificado o tempo de execução para 2 ou 4 threads.

#####Instância com 12 cidades:

	   0 300 352 466 217 238 431 336 451  47 415 515
	 300   0 638 180 595 190 138 271 229 236 214 393
	 352 638   0 251  88 401 189 386 565 206 292 349
	 466 180 251   0 139 371 169 316 180 284 206 198
	 217 595  88 139   0 310 211 295 474 130 133 165
	 238 190 401 371 310   0 202 122 378 157 362 542
	 431 138 189 169 211 202   0 183  67 268 117 369
	 336 271 386 316 295 122 183   0 483 155 448 108
	 451 229 565 180 474 378  67 483   0 299 246 418
	  47 236 206 284 130 157 268 155 299   0 202 327
	 415 214 292 206 133 362 117 448 246 202   0 394
	 515 393 349 198 165 542 368 108 418 327 394   0
	 

	2 threads
	
	[inf2591-06@server parallel-tsp]$ mpirun -np 1 --hostfile host_file ./main
	Cities number:
	12
	
	BEST TOUR:
	Best tour: 1733.00
	Total execution time: 2.10s
	-------------------------------------
	4 threads
	
	[inf2591-06@server parallel-tsp]$ mpirun -np 1 --hostfile host_file ./main
	Cities number:
	12
	
	BEST TOUR:
	Best tour: 1733.00
	Total execution time: 2.69s
