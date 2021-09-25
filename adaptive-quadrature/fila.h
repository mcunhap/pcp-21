typedef struct fila Fila;

void CriarFila( struct Fila *f, int c );

// insere o item do inicio da fila
void InsereTarefa(struct Fila *f, int a, int b);

// retira o item do inicio da fila
void RetiraTarefa(struct Fila *f, float* result);

// retorna verdadeiro com a fila vazia
int Vazia(struct Fila *f);

// retorna verdadeiro com a fila cheia
int Cheia(struct Fila *f);

// retorna verdadeiro com a fila vazia
void ExibeFila(struct Fila *f);