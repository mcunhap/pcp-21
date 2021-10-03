# Quadratura Adaptativa com openMP

## Descrição
Nesse trabalho implementamos duas variantes do [algoritmo da quadratura adaptativa](http://dl.acm.org/citation.cfm?id=103164), utilizando OpenMP. Fizemos também uma implementação na primeira variante usando somente pthreads. O bojetivo era comparar os resultados de tempos de execução.

## Implementação
O programa utiliza **n** threads para computar a aproximação, pelo **método de trapézios**, da **área abaixo da curva** formada por uma função f. O número de trapézios a ser calculado para realizar a aproximação depende de uma tolerância. Para determinado intervalo `[a,b]`, o algoritmo calcula a área do trapézio com extremidades `(a, b, f(a), f(b))`, e a área dos dois subtrapézios formados por `(a, (b-a)/2, f(a), f((b-a)/2))` e por `((b-a)/2, b, f((b-a)/2), f(b))`. Se a diferença entre a área do trapézio maior e a soma dos dois subtrapézios fica abaixo de uma tolerância, o cálculo da área entre a e b está completo. Caso contrário, deve-se repetir o mesmo procedimento para os trapézios menores. A tolerância na diferença entre um trapézio e os dois subtrapézios "seguintes" deve ser uma porcentagem da soma de áreas dos dois subtrapézios.

1. Na primeira implementação, cada thread calcula um subintervalo pelo qual será responsável e calcula o resultado para esse subintervalo inteiro. Quando todas as threads terminarem, a thread principal deve mostrar o resultado final. Essa variante deve ser implementada com pthreads e com OpenMP.

2. Na segunda variante, a thread principal inicialmente cria uma lista de tarefas, contendo os extremos dos intervalos, com `NUMINICIAL` tarefas. Cada thread executa uma tarefa, e se ela gerar subtarefas, coloca uma delas na fila global e processa a outra, até que não encontre mais tarefas na fila (Escreva operaçoes `InsereTarefa` e `RetiraTarefa` para manipular essa fila.). A thread principal espera as demais terminarem e mostra o resultado final. Essa variante deve ser implementada apenas com OpenMP.

## Setup
1. [Instalar Docker](https://docs.docker.com/engine/install/)
2. Executar cenário desejado
- Cenário base
```
make run-simple
```
- Cenário utilizando omp
```
make run-omp
```
- Cenário utilizando fila de tarefas
```
make run-tasks
```