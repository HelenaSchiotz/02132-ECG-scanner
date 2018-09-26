#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main() {
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");	//Open file ECG.txt
	int arraySize = 10000;
    int* rawDataArray = malloc(arraySize*sizeof(int)); //Declare int array with 32 ints
    int* LPFDataArray = malloc(arraySize*sizeof(int));
    int* HPFDataArray = malloc(arraySize*sizeof(int));
    int* derivativeArray = malloc(arraySize*sizeof(int));
    int* squareArray = malloc(arraySize*sizeof(int));
    int* MWIArray = malloc(arraySize*sizeof(int));
    int* peaks = malloc(arraySize*sizeof(int));

    //Initializing params and its variables
    QRS_params *params;
    params->Rpeak = 0;
    params->SPKF = 4225;
	params->NPKF = 0;
	params->THRESHOLD1 = 5000;
	params->THRESHOLD2 = 600;
	params->RR_Average1 = 0;
	params->RR_Average2 = 0;
	params->RR_LOW = 110;
	params->RR_HIGH = 180;
	params->RR_MISS = 200;
	params->RecentRR_OK = malloc(8*sizeof(int));
	params->RecentRR = malloc(8*sizeof(int));
	params->RR = 0;
	params->peak = 0;
	params->SincePeak = 0;
	params->count = -1;
	params->missed = 0;
	params->RecentRR_RR = malloc(arraySize*sizeof(int));


	for (int i = 0; i < 8; i++) {
		params->RecentRR_OK[i] = 140;
		params->RecentRR[i] = 100;
	}

    for(int i = 0; i < arraySize; i++){ //Set all indices to 0
    	rawDataArray[i] = 0;
    	LPFDataArray[i] = 0;
    	HPFDataArray[i] = 0;
    	derivativeArray[i] = 0;
    	squareArray[i] = 0;
    	MWIArray[i] = 0;
    	peaks[i] = 0;
    	params->RecentRR_RR[i] = 0;
    }

    for(int i = 0; i < arraySize; i++){
		shuffleArray(rawDataArray,arraySize ,getNextData(file));
		shuffleArray(LPFDataArray, arraySize, lowPassFilter(rawDataArray,LPFDataArray));
		shuffleArray(HPFDataArray,arraySize,highPassFilter(LPFDataArray,HPFDataArray));
		shuffleArray(derivativeArray,arraySize,derivative(HPFDataArray));
		shuffleArray(squareArray,arraySize,squaring(derivativeArray[0]));
		shuffleArray(MWIArray,arraySize,MWI(squareArray));
		//printf("%i\n",MWIArray[0]);
	    int p = peakDetection(MWIArray, peaks, params); // Perform Peak Detection
	    //printf("peak %i, RR %i, Average1 %i, threshold1 %i  \n", params->peak, params->RR, params->RR_Average1,params->THRESHOLD1);
    }


	return 0;
}


void shuffleArray(int* array, int arrayCount, int newData){
	int pH;
	for (int i = arrayCount-1; i > 0; i--) {
		pH = array[i - 1];
		array[i] = pH;
	}
	array[0] = newData;
}
