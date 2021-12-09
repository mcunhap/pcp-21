/*
 * =====================================================================================
 *
 *       Filename:  deque.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/12/2021 22:23:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pthread.h>
#include "circular_array.h"

typedef struct deque_t deque;

deque* CreateDeque(int capacity);

int GetTop(deque* deque_t);

int EmptyDeque(deque* deque_t);

void PushBottomDeque(deque* deque_t, tour* tour_t);

tour* PopTopDeque(deque* deque_t);

tour* PopBottomDeque(deque* deque_t);

void PrintDeque(deque* deque_t);
