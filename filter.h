// Header:	  	filter processor to calculate vehicle IDs
// File Name: 	filter.h
// Author:	  	Rudi van Schalkwyk
// Date:		12/11/2022

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <stdint.h>

typedef struct position
{
    float       latitude;
    float       longitude;
}__attribute__ ((packed)) position_t;

typedef struct location
{
    position_t  position;
    uint32_t    closest_id;
}location_t;

typedef struct vehicle
{
    uint32_t    id;
    char        registration[10];
    position_t  position;
    uint64_t    timestamp;
}__attribute__ ((packed)) vehicle_t;    // pack the structure for byte allignment to force 30 bytes size

typedef struct filter_handle
{
    vehicle_t   *vehicle_p;
    location_t  *location_p;
}filter_handle_t;

extern void FILTER_init(filter_handle_t *filter, FILE *datafile_p, int numberOfrecords);
extern void FITLER_process(filter_handle_t *filter, int numberOfrecords);

#endif // FILTER_H_INCLUDED
