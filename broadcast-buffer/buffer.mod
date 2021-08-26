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
int next_data = 0;
int falta_ler[N]; // cada posicao iniciada com zero

SIGNAL
if (pb > 0 && falta_ler[next_free] == 0)
  pb--; V(sp);
else if (cb > 0 && falta_ler[next_data] > 0)
  cb--; V(sc);
else
  V(e);


void deposita(tbuffer* buffer, int item) {
  /* <await (falta_ler[next_free] > 0) falta_ler[next_free] = buffer->numcons> */
  P(e);
  if (falta_ler[next_free] > 0) { pb++; V(e); P(sp); }
  falta_ler[next_free] = buffer->numcons;
  SIGNAL

  /* produce */
  buffer[next_free] = data

  /* <next_free = (next_free + 1) % N> */
  P(e);
  next_free = (next_free + 1) % N;
  SIGNAL
}

int consome(tbuffer* buffer, int meuid) {
  /* <await (falta_ler[next_data] == 0) falta_ler[next_data]--> */
  P(e);
  if (falta_ler[next_data] == 0) { cb++; V(e); P(sc); }
  falta_ler[next_data] = falta_ler[next_data] - 1;
  SIGNAL

  /* consume */
  data = buffer[next_data]

  /* <next_data = (next_data + 1) % N> */
  P(e);
  if (falta_ler[next_data] == 0)
    next_data = (next_data + 1) % N;
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
    //thread_id?
    consome(buffer, thread_id);
  }
}
