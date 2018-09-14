#include "filters.h"

// Feel free to change return statement and arguments
int lowPassFilter(int* rawData, int* LPFFiltered){
int value = 2*LPFFiltered[0]-LPFFiltered[1]+(rawData[0]-2*rawData[6]+rawData[12])/32;
return value;
}

int highPassFilter(int* LPFFiltered, int* HPFFiltered){
	int value = HPFFiltered[0]-LPFFiltered[0]/32+LPFFiltered[16]-LPFFiltered[17]+LPFFiltered[32]/32;
	return value;
}

int derivative(int* unfilteredArray){
	int value = ((2*unfilteredArray[0]+unfilteredArray[1]-unfilteredArray[3]-2*unfilteredArray[4]))/8;
	return value;
}

int squaring(int data){
	return data*data;
}

int MWI(int* unfilteredArray){
	int sum = 0;
	for(int i = 0; i < 33; i++){
		sum += unfilteredArray[i];
	}
	return sum/30;
}
