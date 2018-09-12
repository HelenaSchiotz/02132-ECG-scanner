#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main()
{	
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");	//Open file ECG.txt
    int* unfilteredDataArray = malloc(32*sizeof(int)); //Declare int array with 32 ints
    int* filteredDataArray = malloc(2*sizeof(int)); //Declare int array with 2 ints


    for(int i = 0; i < 32; i++){ //Set all indices to 0
    	unfilteredDataArray[i] = 0;
    }
    for(int i = 0; i < 2; i++){ //Set all indices to 0
        filteredDataArray[i] = 0;
    }
    for(int i = 0; i < 35; i++){
    int data = getNextData(file);          // Read Data from Sensor

    int LPF = lowPassFilter(data,unfilteredDataArray,filteredDataArray);  // Filter Data with lowpass
    int HPF = highPassFilter(LPF,unfilteredDataArray,filteredDataArray);
    int D = derivative(HPF,unfilteredDataArray);
    int sq = squaring(D);
    int mw = MWI(sq,unfilteredDataArray);

    unfilteredDataArray = shuffleArray(unfilteredDataArray, 32, data);
    filteredDataArray = shuffleArray(filteredDataArray,2 , mw);
    }
    //peakDetection(&qsr_params); // Perform Peak Detection

	return 0;
}


int* shuffleArray(int* array, int arrayCount, newData){

	return 0;
}
