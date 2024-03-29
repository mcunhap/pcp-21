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

stack* CreateStack(int max_size);

void FreeStack(stack* stack_t);

void PushCopy(stack* stack_t, tour* tour_t);

tour* Pop(stack* stack_t);

stack* SplitStack(stack* src_stack);

void CopyStack(stack* orig, stack* dest);

int Empty(stack* stack_t);

tour* GetLastTour(stack* stack_t);

int GetSize(stack* stack_t);

int GetMaxSize(stack* stack_t);

// Just for debugging
void PrintStackInfo(stack* stack_t);
