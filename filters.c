#include "filters.h"

// Feel free to change return statement and arguments
int lowPassFilter(int data, int* unfilteredArray, int* filteredArray){
int value = 2*filteredArray[0]-filteredArray[1]+(data-2*unfilteredArray[5]+unfilteredArray[11])/32;
return value;
}

int highPassFilter(int data,int* unfilteredArray, int* filteredArray){
	int value = filteredArray[0]-(data/32)+unfilteredArray[15]-unfilteredArray[16]+(unfilteredArray[31]/32);
	return value;
}

int derivative(int data, int* unfilteredArray){
	int value = ((2*data+unfilteredArray[0]-unfilteredArray[2]-2*unfilteredArray[3]))/8;
	return value;
}

int squaring(int data){
	return data*data;
}

int MWI(int data, int* unfilteredArray){
	int sum = 0;
	for(int i = 0; i < 32; i++){
		sum += unfilteredArray[i];
	}
	sum += data;
	return sum/30;
}
