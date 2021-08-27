tbuffer* buffer = iniciabuffer(N, P, C);

/*
O inteiro next_free controla qual a próxima posição vazia do buffer para os produtores inserirem o dado.
*/
int next_free = 0;

/*
O array next_data controla qual a próxima posição do buffer que cada consumidor deve ler o dado.
ps: deve ser iniciado tudo com zero
*/
int next_data[C];

/*
A matrix falta_ler é um auxiliar para saber qual consumidor consumiu de cada uma das posições. Para isso ele tem tamanho NxC, onde N é o tamanho do Buffer e C é o número de consumidores. Sempre que um produtor consumir ele precisa preencher todas C posições da próxima posição livre com 1. Sempre que um consumidor consumir ele precisa transformar tornar a sua posição C em zero.

Por exemplo:
N= 4, C=3

Quando o buffer todo esta cheio, o array falta_ler fica representado da seguinte maneira:
[1 1 1 1]
[1 1 1 1]
[1 1 1 1]

Quando C1 consumiu do buffer cheio, falta_ler fica representado da seguinte maneira:
[0 1 1 1]
[1 1 1 1]
[1 1 1 1]
*/
int falta_ler[N][C];

/*
O Produtor deve aguardar o array falta_ler na próxima posição livre ter todas posições vazias. Depois ele preenche todas posições do falta_ler[next_free] com 1. Com isso ele pode produzir, e no final faz a operação atomica para incrementar a proxima posição livre do buffer.
ps: metodos auxiliares is_empty e fill no final do arquivo
*/

void deposita(tbuffer* buffer, int item) {
  <await (is_empty(falta_ler[next_free]) fill(falta_ler[next_free], 1)>

  /* produz */
  buffer[next_free] = item

  <next_free = (next_free + 1) % N>
}

/*
O Consumidor deve aguardar o array falta_ler na próxima posição dele de leitura, na posição dele de consumo ficar maior que zero (isso representa que tem dado para consumir nessa posição e que ele ainda não consumiu). Depois ele precisa setar essa posição para zero, indicando que ele consumiu e então consumir. Por final ele faz uma operação atomica para incrementar a proxima posição de leitura de dado dele.
*/

int consome(tbuffer* buffer, int meuid) {
  <await (falta_ler[next_data[meuid]][meuid] > 0) falta_ler[next_data[meuid]][meuid] == 0>

  /* consome */
  print(buffer[next_data[meuid]])

  <next_data[meuid] = (next_data[meuid] + 1) % N>
}

/*
Procesos dos produtores e consumidores, que seriam as threads. Cada um executa I vezes, assim cada produtor irá produzir I e, como temos um Broadcast, cada consumidor consumirá I*P vezes.
*/
process Producer {
  int data = rand();

  for (int i=0; i < I; i++) {
    deposita(buffer, data);
  }
}

process Consumer {
  for (int i=0; i < I; i++) {
    consome(buffer, thread_id);
  }
}

/*
Metodos auxiliares para verificação e modificações feitas pelos produtores
*/

int is_empty(int [] array) {
  for(int i=0; i<sizeof(array); i++) {
    if(array[i] > 0) { return false; }
  }

  return true;
}

void fill(int [] array, int value) {
  for(int i=0; i<sizeof(array); i++) {
    array[i] = value;
  }
}
