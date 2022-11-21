// Header:	  	filter processor to calculate vehicle IDs
// File Name: 	filter.c
// Author:	  	Rudi van Schalkwyk
// Date:		12/11/2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xmmintrin.h>
#include "filter.h"

void FILTER_init(filter_handle_t *filter, FILE *datafile_p, int numberOfrecords);
void FITLER_process(filter_handle_t *filter, int numberOfrecords);
float distance(float x1, float x2, float y1, float y2);

/*******************************************************************************
* Function Name : FILTER_init
* Description : allocates memmory from the heap for the filter datapoints, reads input file into allocated memmory
* Input : handle of type filter_handle_t * initialised by calling function, file pointer datafile_p to source file, interger for number of datapoints to process
* Output : debug terminal prints of memmory allocation and file parsing success
* Return : None
*******************************************************************************/
void FILTER_init(filter_handle_t *filter, FILE *datafile_p, int numberOfrecords)
{
	int readsize = 0;

    filter->vehicle_p = calloc(numberOfrecords, sizeof(vehicle_t));     // allocate space on the heap for the entire file
    if(filter->vehicle_p == NULL)
    {
        fprintf(stderr, "failed to allocate memory for vehicles\n");

		free(filter->vehicle_p);
		exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "memmory allocated for input file \n");
    }

    filter->location_p = calloc(10, sizeof(location_t));                // allocate space on the heap for the 10 locations
    if(filter->location_p == NULL)
    {
        fprintf(stderr, "failed to allocate memory for locations\n");

		free(filter->location_p);
		exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "memmory allocated for locations \n");
    }


    fseek(datafile_p, 0, SEEK_SET);                                     // take us ot the start of the file
  	readsize = fread(filter->vehicle_p, sizeof(vehicle_t), numberOfrecords, datafile_p);    // parse the file in chunks of vehicle_t
    fprintf(stdout, "%d elements read \n", readsize);

    filter->location_p[0].position.latitude = 34.544909;                   // load the location data points into memmory
    filter->location_p[0].position.longitude = -102.100843;

    filter->location_p[1].position.latitude = 32.345544;
    filter->location_p[1].position.longitude = -99.123124;

    filter->location_p[2].position.latitude = 33.234235;
    filter->location_p[2].position.longitude = -100.214124;

    filter->location_p[3].position.latitude = 35.195739;
    filter->location_p[3].position.longitude = -95.348899;

    filter->location_p[4].position.latitude = 31.895839;
    filter->location_p[4].position.longitude = -97.789573;

    filter->location_p[5].position.latitude = 32.895839;
    filter->location_p[5].position.longitude = -101.789573;

    filter->location_p[6].position.latitude = 34.115839;
    filter->location_p[6].position.longitude = -100.225732;

    filter->location_p[7].position.latitude = 32.335839;
    filter->location_p[7].position.longitude = -99.992232;

    filter->location_p[8].position.latitude = 33.535339;
    filter->location_p[8].position.longitude = -94.792232;

    filter->location_p[9].position.latitude = 32.234235;
    filter->location_p[9].position.longitude = -100.222222;

    return;
}

/*******************************************************************************
* Function Name : FITLER_process
* Description : Application to calculate vehicle IDs closest to point locations
* Usage : Create a handle filter_handle_t *, then initialise wiht call to FILTER_init(), then execute FILTER_process() for results
* Input : handle filter_handle_t * initialised by calling function, interger for number of datapoints to process
* Output : terminal printout of results - vehicle ID closest to 10 locations
* Return : None
*******************************************************************************/
void FITLER_process(filter_handle_t *filter, int numberOfrecords)
{
    float rangeInit[10] = {0};      // used to load initial distances for smallest comparisson
    float rangeComp[10] = {0};      // used to save distance results for smallest comparisson

    // pre-load rangeInit with a arbitratry results based on actual data
    rangeInit[0] = distance(filter->location_p[0].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[0].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[1] = distance(filter->location_p[1].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[1].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[2] = distance(filter->location_p[2].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[2].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[3] = distance(filter->location_p[3].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[3].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[4] = distance(filter->location_p[4].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[4].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[5] = distance(filter->location_p[5].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[5].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[6] = distance(filter->location_p[6].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[6].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[7] = distance(filter->location_p[7].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[7].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[8] = distance(filter->location_p[8].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[8].position.longitude, filter->vehicle_p[0].position.longitude);
    rangeInit[9] = distance(filter->location_p[9].position.latitude, filter->vehicle_p[0].position.latitude, filter->location_p[9].position.longitude, filter->vehicle_p[0].position.longitude);


    // loop through the whole records dataset to calculate distance
    for(int i = 0; i < numberOfrecords; i++ )
    {
        // evaluate the distance for each of the 10 locations against each data point - for() loop ommitted to save valueable executeion time
        rangeComp[0] = distance(filter->location_p[0].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[0].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[0] < rangeInit[0])     // compare results wiht previous results
        {
            filter->location_p[0].closest_id = filter->vehicle_p[i].id;     // update the results
            rangeInit[0] = rangeComp[0];
        }

        rangeComp[1] = distance(filter->location_p[1].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[1].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[1] < rangeInit[1])
        {
            filter->location_p[1].closest_id = filter->vehicle_p[i].id;
            rangeInit[1] = rangeComp[1];
        }

        rangeComp[2] = distance(filter->location_p[2].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[2].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[2] < rangeInit[2])
        {
            filter->location_p[2].closest_id = filter->vehicle_p[i].id;
            rangeInit[2] = rangeComp[2];
        }

        rangeComp[3] = distance(filter->location_p[3].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[3].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[3] < rangeInit[3])
        {
            filter->location_p[3].closest_id = filter->vehicle_p[i].id;
            rangeInit[3] = rangeComp[3];
        }

        rangeComp[4] = distance(filter->location_p[4].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[4].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[4] < rangeInit[4])
        {
            filter->location_p[4].closest_id = filter->vehicle_p[i].id;
            rangeInit[4] = rangeComp[4];
        }

        rangeComp[5] = distance(filter->location_p[5].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[5].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[5] < rangeInit[5])
        {
            filter->location_p[5].closest_id = filter->vehicle_p[i].id;
            rangeInit[5] = rangeComp[5];
        }

        rangeComp[6] = distance(filter->location_p[6].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[6].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[6] < rangeInit[6])
        {
            filter->location_p[6].closest_id = filter->vehicle_p[i].id;
            rangeInit[6] = rangeComp[6];
        }

        rangeComp[7] = distance(filter->location_p[7].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[7].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[7] < rangeInit[7])
        {
            filter->location_p[7].closest_id = filter->vehicle_p[i].id;
            rangeInit[7] = rangeComp[7];
        }

        rangeComp[8] = distance(filter->location_p[8].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[8].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[8] < rangeInit[8])
        {
            filter->location_p[8].closest_id = filter->vehicle_p[i].id;
            rangeInit[8] = rangeComp[8];
        }

        rangeComp[9] = distance(filter->location_p[9].position.latitude, filter->vehicle_p[i].position.latitude, filter->location_p[9].position.longitude, filter->vehicle_p[i].position.longitude);
        if(rangeComp[9] < rangeInit[9])
        {
            filter->location_p[9].closest_id = filter->vehicle_p[i].id;
            rangeInit[9] = rangeComp[9];
        }
    }

    for(int i = 0; i < 10; i++)
    {
        printf("closest vehicle to location #%u is %u \n", i+1, filter->location_p[i].closest_id);  // print the results
    }

    return;
}

/*******************************************************************************
* Function Name : distance
* Description : calculates the absolute distance between two set of coordinates, assumes a flat surface. Applies Pythagoras
* Input : floating point values for coordidate x1 - latitude #1, x2 - latitude #1, y1 - longitude #1, y2 - longituyde #2
* Output : none
* Return : float value of diagonal distnance
*******************************************************************************/
float distance(float x1, float x2, float y1, float y2)
{
    float xx = 0;
    float yy = 0;

    xx = (x2 - x1);
    yy = (y2 - y1);

    xx = xx * xx;
    yy = yy * yy;

    return sqrtf(xx + yy);
}
