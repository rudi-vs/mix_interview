// Header:	  	filter processor to calculate vehicle IDs
// File Name: 	filter.c
// Author:	  	Rudi van Schalkwyk
// Date:		12/11/2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <windows.h>
#include "filter.h"

#define NO_OF_THREADS   10
#define NO_OF_LOCATIONS 10
#define NO_OF_RECORDS   2000000

void FILTER_init(filter_handle_t *filter, FILE *datafile_p);
void FITLER_process(filter_handle_t *filter);
float distance(float x1, float x2, float y1, float y2);
DWORD WINAPI distance_thread(LPVOID arg);

/*******************************************************************************
* Function Name : FILTER_init
* Description : allocates memmory from the heap for the filter datapoints, reads input file into allocated memmory
* Input : handle of type filter_handle_t * initialised by calling function, file pointer datafile_p to source file, interger for number of datapoints to process
* Output : debug terminal prints of memmory allocation and file parsing success
* Return : None
*******************************************************************************/
void FILTER_init(filter_handle_t *filter, FILE *datafile_p)
{
    filter->vehicle_p = (vehicle_t *)malloc(NO_OF_RECORDS * sizeof(vehicle_t));     // allocate space on the heap for the entire file
    if(filter->vehicle_p == NULL)
    {
        fprintf(stderr, "failed to allocate memory for vehicles\n");

		free(filter->vehicle_p);
		exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "memory allocated for input file \n");
    }

    filter->location_p = (location_t *)malloc(NO_OF_LOCATIONS * sizeof(location_t)); // allocate space on the heap for the 10 locations
    if(filter->location_p == NULL)
    {
        fprintf(stderr, "failed to allocate memory for locations\n");

		free(filter->location_p);
		exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "memory allocated for locations \n");
    }

    //fseek(datafile_p, 0, SEEK_SET);                                                // take us ot the start of the file
  	fread(filter->vehicle_p, sizeof(vehicle_t), NO_OF_RECORDS, datafile_p);          // parse the file in chunks of vehicle_t

    filter->location_p[0].pos.lat = 34.544909f;                                      // load the location data points into memmory
    filter->location_p[0].pos.lon = -102.100843f;
    filter->location_p[0].no = 1;

    filter->location_p[1].pos.lat = 32.345544f;
    filter->location_p[1].pos.lon = -99.123124f;
    filter->location_p[1].no = 2;

    filter->location_p[2].pos.lat = 33.234235f;
    filter->location_p[2].pos.lon = -100.214124f;
    filter->location_p[2].no = 3;

    filter->location_p[3].pos.lat = 35.195739f;
    filter->location_p[3].pos.lon = -95.348899f;
    filter->location_p[3].no = 4;

    filter->location_p[4].pos.lat = 31.895839f;
    filter->location_p[4].pos.lon = -97.789573f;
    filter->location_p[4].no = 5;

    filter->location_p[5].pos.lat = 32.895839f;
    filter->location_p[5].pos.lon = -101.789573f;
    filter->location_p[5].no = 6;

    filter->location_p[6].pos.lat = 34.115839f;
    filter->location_p[6].pos.lon = -100.225732f;
    filter->location_p[6].no = 7;

    filter->location_p[7].pos.lat = 32.335839f;
    filter->location_p[7].pos.lon = -99.992232f;
    filter->location_p[7].no = 8;

    filter->location_p[8].pos.lat = 33.535339f;
    filter->location_p[8].pos.lon = -94.792232f;
    filter->location_p[8].no = 9;

    filter->location_p[9].pos.lat = 32.234235f;
    filter->location_p[9].pos.lon = -100.222222f;
    filter->location_p[9].no = 10;

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
void FITLER_process(filter_handle_t *filter)
{
    DWORD ThreadId[NO_OF_THREADS];
    HANDLE ThreadHandle[NO_OF_THREADS];
    thread_param_t *thread;                            // application parameters to be passed to each thread

    thread = (thread_param_t *)malloc(NO_OF_THREADS * sizeof(thread_param_t));    // allocate memoryt on the heap for a tread parameters
    if(thread == NULL)
    {
        ExitProcess(EXIT_FAILURE);
    }

    for(register int i = 0; i < NO_OF_THREADS ; i++)
    {
        thread[i].filter = filter;                      // each thread paramter set get a copy of the handler(pointers to all data memory)
        thread[i].thread_no = i;                        // each thread is numbered 0 - 9, each thread will process on one location
    }

    for(register int l = 0; l < NO_OF_THREADS ; l++)
    {
        ThreadHandle[l] = CreateThread( NULL, 0, distance_thread, &thread[l], 0, &ThreadId[l]); // spawn a thread for each of the 10 locations
        if (ThreadHandle[l] == NULL)
        {
            ExitProcess(EXIT_FAILURE);
        }
    }

    WaitForMultipleObjects(NO_OF_THREADS, ThreadHandle, TRUE, INFINITE);        // wait for the threads the end

    for(register int i = 0; i < NO_OF_THREADS ; i++)
    {
        CloseHandle(ThreadHandle[i]);                    // release the win32 handle
    }

    for(register int i = 0; i < NO_OF_THREADS ; i++)
    {
        fprintf(stdout, "closest vehicle to location #%u is %u \n", filter->location_p[i].no, filter->location_p[i].closest_id);  // print the results
    }
	
    free(thread);

    return;
}

/*******************************************************************************
* Function Name : distance
* Description : calculates the absolute distance between two set of coordinates, assumes a flat surface. Applies Pythagoras
* Input : floating point values for coordidate x1 - lat #1, x2 - lat #1, y1 - lon #1, y2 - lonituyde #2
* Output : none
* Return : float value of diagonal distnance
*******************************************************************************/
float distance(float x1, float x2, float y1, float y2)
{
    register float xx = 0;
    register float yy = 0;

    xx = (x2 - x1);
    yy = (y2 - y1);

    xx = xx * xx;
    yy = yy * yy;

    return sqrtf(xx + yy);
}

/*******************************************************************************
* Function Name : distance_thread
* Description : A function called by FILTER_process, to be spawned into mutlipler threads. Each thread processes one location
* Input : a void pointer, expects a pointer of type thread_param_t, which includes a data handle and id of the location to process
* Output : processed data is stored in "handle->filter->location_p[id].closest_id"
* Return : none
*******************************************************************************/
DWORD WINAPI distance_thread(LPVOID arg)
{
    thread_param_t *handle = (thread_param_t*)arg;

    register float rangeInit = 0;
	register float rangeComp = 0;
	register int id = handle->thread_no;

	// pre-load rangeInit with a arbitratry results based on actual data
    rangeInit = distance(handle->filter->location_p[id].pos.lat, handle->filter->vehicle_p[0].pos.lat, handle->filter->location_p[id].pos.lon, handle->filter->vehicle_p[0].pos.lon);

    for(register int i = 0; i < NO_OF_RECORDS; i++ )
    {
        rangeComp = distance(handle->filter->location_p[id].pos.lat, handle->filter->vehicle_p[i].pos.lat, handle->filter->location_p[id].pos.lon, handle->filter->vehicle_p[i].pos.lon);
        if(rangeComp < rangeInit)     // compare results wiht previous results
        {
            handle->filter->location_p[id].closest_id = handle->filter->vehicle_p[i].id;     // update the results
            rangeInit = rangeComp;
        }
    }

    return 0;
}
