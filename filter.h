// Header:	  	filter processor to calculate vehicle IDs
// File Name: 	filter.h
// Author:	  	Rudi van Schalkwyk
// Date:		12/11/2022

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <stdint.h>

typedef struct position
{
    float       lat;
    float       lon;
}__attribute__ ((packed)) position_t;

typedef struct location
{
    position_t  pos;
    uint32_t    closest_id;
    uint32_t    no;
}location_t;

typedef struct vehicle
{
    uint32_t    id;
    char        reg[10];
    position_t  pos;
    uint64_t    timestamp;
}__attribute__ ((packed)) vehicle_t;    // pack the structure for byte allignment to force 30 bytes size

typedef struct filter_handle
{
    vehicle_t   *vehicle_p;
    location_t  *location_p;
}filter_handle_t;

typedef struct thread_param
{
    filter_handle_t *filter;
    int         thread_no;
}thread_param_t;

extern void FILTER_init(filter_handle_t *filter, FILE *datafile_p);
extern void FITLER_process(filter_handle_t *filter);

#endif // FILTER_H_INCLUDED

