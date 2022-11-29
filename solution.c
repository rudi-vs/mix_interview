// Header:	  	Application to measure processor execution time for calculating vehicle IDs closest to point locations
// File Name: 	solution.c
// Author:	  	Rudi van Schalkwyk
// Date:		12/11/2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h> // stat
#include <time.h>
#include "filter.h"

FILE *datafile_p = NULL;

/*******************************************************************************
* Function Name : main
* Description : Application to measure processor execution time for calculating vehicle IDs closest to point locations
* Input : positions.dat is hard code and expected in root directory
* Output : terminal printout of exection time in seconds
* Return : None
*******************************************************************************/
int main(void)
{
    clock_t start_t, end_t;
    double total_t;

    filter_handle_t filter_h;                               // create instance of the filter function handle

    start_t = clock();                                      // start the timer

	datafile_p = fopen("positions.dat", "rb");
	if(datafile_p == NULL)                                  // some error handling incase the inout file does not exist
    {
        fprintf(stderr, "input file not found\n");
        exit(EXIT_FAILURE);
    }

    FILTER_init(&filter_h, datafile_p);                     // initialite filter datastructures

    FITLER_process(&filter_h);                              // process the records

    end_t = clock();                                        // stop the clock
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    fprintf(stdout, "Total time taken by CPU: %f seconds\n", total_t);

    free(filter_h.location_p);                              // free the handle from the heap
    free(filter_h.vehicle_p);

    fclose(datafile_p);
}
