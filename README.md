# Vehicle Position Challenge - Rudi van Schalkwyk submission

## Design
The design consist of a module FILTER_init() which allocates memory on the heap based on the supplied file size. FILTER_process() spawns a new thread for each of the 10 locations. The thread processes the 2 000 000 to get the closeset vehicle id.

The distance between the location and record sample is calculated using Pythagoras. This is the simplest mathematical method, saving valuable processor time at the expense of some accuracy.
Calculating the distance in actual measurement units is omitted as the goal lies with calculating the closest vehicle and not the resulting distance. This is done to save processing time.

## Assumptions
From the specifics provided: 2 million records and a file size of 60 000 000 bytes it is assumed that records are byte aligned and all the same size, including the vehicle registration. 
Therefore, the record size may be calculated at 30 bytes = 60 000 000 / 2 000 000. This allows for a maximum size of 10 bytes for the vehicle registration string, null terminator included.
