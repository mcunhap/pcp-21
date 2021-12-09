/*
 * =====================================================================================
 *
 *       Filename:  circular_array.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/12/2021 20:18:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tour.h"

typedef struct circular_array_t circular_array;

circular_array* CreateCircularArray(int capacity);

tour* Get(circular_array* c_array, int i);

void Put(circular_array* c_array, int i, tour* tour);

void FreeCircularArray(circular_array* c_array);

int GetCapacity(circular_array* c_array);

circular_array* Resize(circular_array* c_array, int bottom, int top);

void PrintCArray(circular_array* c_array, int bottom, int top);
