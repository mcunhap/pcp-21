/*
 * =====================================================================================
 *
 *       Filename:  tour.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2021 18:43:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
typedef struct tour_t tour;

tour* CreateTour();

int* GetTourCities(tour* tour_t);

int GetTourNumberCities(tour* tour_t);

int GetTourCost(tour* tour_t);
