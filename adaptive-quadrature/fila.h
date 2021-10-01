typedef struct elemento Elemento;

typedef struct fila Fila;

Fila* CriarFila();

// insere o item do inicio da fila
void InsereTarefa(Fila *f, double a, double b);

// retira o item do inicio da fila
Elemento *RetiraTarefa(Fila *f);

// retorna verdadeiro com a fila vazia
int Vazia(Fila *f);

// printa fila
void ExibeFila(Fila *f);

// printa elemento
void ExibeElemento(Elemento *e);

// le dada a do elemento
double LeDadoA(Elemento *e);


// le dada b do elemento
double LeDadoB(Elemento *e);
