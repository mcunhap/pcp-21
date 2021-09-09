/*
 * =====================================================================================
 *
 *       Filename:  buffer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/08/2021 21:53:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

typedef struct sbuffer tbuffer;

tbuffer* iniciabuffer (int numpos, int numprod, int numcons);
void deposita (tbuffer* buffer, int item);
int consome (tbuffer* buffer, int meuid);
void finalizabuffer (tbuffer* buffer);
