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

void AddCity(tour* tour_t, int city, float cost);

int LastCity(tour* tour_t);

void RemoveLastCity(tour* tour_t, int cost);

int TourContainCity(tour* tour_t, int city);

int BestTour(tour* tour_t, tour* best);

int* GetTourCities(tour* tour_t);

int GetTourMaxCities(tour* tour_t);

int GetTourNumberCities(tour* tour_t);

int GetTourCost(tour* tour_t);

void CopyTour(tour* dest, tour* orig);

// Just for debugging
void PrintTourInfo(tour* tour_t);
