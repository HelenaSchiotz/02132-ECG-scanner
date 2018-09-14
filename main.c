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
	int arraySize = 33;
    int* rawDataArray = malloc(arraySize*sizeof(int)); //Declare int array with 32 ints
    int* LPFDataArray = malloc(arraySize*sizeof(int));
    int* HPFDataArray = malloc(arraySize*sizeof(int));
    int* derivativeArray = malloc(arraySize*sizeof(int));
    int* squareArray = malloc(arraySize*sizeof(int));
    int* MWIArray = malloc(arraySize*sizeof(int));



    for(int i = 0; i < arraySize; i++){ //Set all indices to 0
    	rawDataArray[i] = 0;
    	LPFDataArray[i] = 0;
    	HPFDataArray[i] = 0;
    	derivativeArray[i] = 0;
    	squareArray[i] = 0;
    	MWIArray[i] = 0;
    }

    for(int i = 0; i < 35; i++){
		shuffleArray(rawDataArray,arraySize ,getNextData(file));
		shuffleArray(LPFDataArray, arraySize, lowPassFilter(rawDataArray,LPFDataArray));
		shuffleArray(HPFDataArray,arraySize,highPassFilter(LPFDataArray,HPFDataArray));
		shuffleArray(derivativeArray,arraySize,derivative(HPFDataArray));
		shuffleArray(squareArray,arraySize,squaring(derivativeArray[0]));
		shuffleArray(MWIArray,arraySize,MWI(squareArray));

    }
    //peakDetection(&qsr_params); // Perform Peak Detection

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
