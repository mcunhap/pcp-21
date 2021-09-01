/*
 * =====================================================================================
 *
 *       Filename:  buffer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/08/2021 21:54:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"

struct sbuffer {
  int numpos;
  int numprod;
  int numcos;
};

tbuffer* iniciabuffer (int numpos, int numprod, int numcos) {
  tbuffer *buffer = calloc(1,sizeof(tbuffer));

  if(!buffer) {
    return NULL;
  }

  buffer->numpos = numpos;
  buffer->numprod = numprod;
  buffer->numcos = numcos;

  return buffer;
}

void finalizabuffer (tbuffer* buffer) {
  free (buffer);
}

void deposita (tbuffer* buffer, int item) {
  printf("Produziu: %d\n", item);
}

int consome (tbuffer* buffer, int meuid) {
  return 6;
}
