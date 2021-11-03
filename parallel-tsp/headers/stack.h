/*
 * =====================================================================================
 *
 *       Filename:  stack.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2021 21:23:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "tour.h"

typedef struct stack_t stack;

stack* CreateStack(int max_cities);

void FreeStack(stack* stack_t);

void PushCopy(stack* stack_t, tour* tour_t);

tour* Pop(stack* stack_t);

int Empty(stack* stack_t);

tour** GetTours(stack* stack_t);

int GetSize(stack* stack_t);

