#include "filters.h"

// Feel free to change return statement and arguments
int lowPassFilter(int data, int* unfilteredArray, int* filteredArray){
float value = (2*filteredArray[0])-filteredArray[1]+((1.0/32.0)*(data-2*unfilteredArray[5]+unfilteredArray[11]));
return (int) value;
}

int highPassFilter(int data,int* unfilteredArray, int* filteredArray){
	float value = filteredArray[0]-data/32.0+unfilteredArray[15]-unfilteredArray[16]+unfilteredArray[31]/32.0;
	return (int) value;
}

int derivative(int data, int* unfilteredArray){
	float value = (1.0/8.0)*(2*data+unfilteredArray[0]-unfilteredArray[2]-2*unfilteredArray[3]);
	return (int) value;
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
	return (int) (1.0/30.0)*sum;
}
