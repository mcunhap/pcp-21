/* typedef struct sbuffer tbuffer; */

/* tbuffer* iniciabuffer (int numpos, int numprod, int numcons); */
/* void deposita (tbuffer* buffer, int item); */
/* int consome (tbuffer* buffer, int meuid); */
/* void finalizabuffer (tbuffer* buffer); */

tbuffer* buffer = iniciabuffer(N, P, C);
sem_t e;
sem_t sp; // semaforo produtor
sem_t sc; // semaforo consumidor

int pb = 0; // produtor bloqueado
int cb = 0; // consumidor bloqueado

int next_free = 0;
int next_data[C]; // cada pos iniciada com zero

/*
Largura: N; Altura: C
[1 1 1 1]
[1 1 1 1]
[1 1 1 1]
*/
int falta_ler[N][C];

/*
SIGNAL
if (pb > 0 && falta_ler[next_free] == 0)
  pb--; V(sp);
else if (cb > 0 && falta_ler[next_data] > 0)
  cb--; V(sc);
else
  V(e);
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

void deposita(tbuffer* buffer, int item) {
  /* <await (is_empty(falta_ler[next_free]) fill(falta_ler[next_free], 1)> */
  P(e);
  if (is_empty(falta_ler[next_free])) { pb++; V(e); P(sp); }
  fill(falta_ler[next_free], 1)
  SIGNAL

  /* produz */
  buffer[next_free] = data

  /* <next_free = (next_free + 1) % N> */
  P(e);
  next_free = (next_free + 1) % N;
  SIGNAL
}

int consome(tbuffer* buffer, int meuid) {
  /* <await (falta_ler[next_data[meuid]][meuid] > 0) falta_ler[next_data[meuid]][meuid] == 0> */
  P(e);
  if (falta_ler[next_data[meuid]][meuid] == 0) { cb++; V(e); P(sc); }
  falta_ler[next_data[meuid]][meuid] = 0;
  SIGNAL

  /* consome */
  data = buffer[next_data]

  /* <next_data[meuid] = (next_data[meuid] + 1) % N> */
  P(e);
  next_data[meuid] = (next_data[meuid] + 1) % N;
  SIGNAL
}


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
