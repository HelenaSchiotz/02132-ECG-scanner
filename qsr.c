#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakDetection(int* MWI, int* peaks, QRS_params *params) {


	if ((MWI[0] < MWI[1]) && (MWI[1] > MWI[2])) {
		params->peak = MWI[1];
		shuffleArray(peaks, 1000, params->peak);
		//printf("%i \n", params->peak);
		if (params->peak > params->THRESHOLD1) {
			params->RR = params->SincePeak;
			params->SincePeak = 0;
			//printf("RR: %i \n", params->RR);

			if ((params->RR_LOW < params->RR) && (params->RR < params->RR_HIGH)) {
				//printf("PEAK");
				params->Rpeak = params->peak;
				params->SPKF = (int) (0.125*params->peak + 0.875*params->SPKF);
				//printf("SPKF: %i", params->SPKF);

				shuffleArray(params->RecentRR_OK, 8, params->RR);
				shuffleArray(params->RecentRR, 8, params->RR);


				params->RR_Average2 = average(params->RecentRR_OK);
				params->RR_Average1 = average(params->RecentRR);

				//printf("A2: %i", params->RR_Average2);
				//printf("A1: %i", params->RR_Average1);

				params->RR_LOW = (int) (0.92*params->RR_Average2);
				params->RR_HIGH = (int) (1.16*params->RR_Average2);
				params->RR_MISS = (int) (1.66*params->RR_Average2);
				params->THRESHOLD1 = params->NPKF  + (int) 0.25*(params->SPKF-params->NPKF);
				params->THRESHOLD2 = (int) 0.5*params->THRESHOLD1;
				printf("%i %i \n", params->count, params->Rpeak);
			} else {
				if (params->RR > params->RR_MISS) {
					int peak2 = searchback(peaks, params->THRESHOLD2);
					//printf("peak2: %i \n", peak2);
					//peak or peak2?????
					params->Rpeak = peak2;
					params->SPKF = (int) ((0.25*peak2) +  (0.75*params->SPKF));
					shuffleArray(params->RecentRR, 8, params->RR);
					params->RR_Average1 = average(params->RecentRR);
					params->RR_LOW = (int) (0.92*params->RR_Average1);
					//printf("LOW: %i ", params->RR_LOW);
					params->RR_HIGH = (int)  (1.16*params->RR_Average1);
					//printf("HIGH: %i ", params->RR_HIGH);
					params->RR_MISS = (int) (1.66*params->RR_Average1);
					//printf("MISS: %i \n", params->RR_MISS);
					params->THRESHOLD1 = (int) (params->NPKF + (0.25*(params->SPKF-params->NPKF)));
					params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
					printf("%i %i \n", params->count, params->Rpeak);
				}
			}

		} else {
			params->NPKF = (int) (0.125*params->peak + 0.875*params->NPKF);
			params->THRESHOLD1 = (int) (params->NPKF + 0.25*(params->SPKF-params->NPKF));
			params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
			//printf("TH1: %i \n", params->THRESHOLD1);
		}
	}
	params->SincePeak += 1;
	params->count += 1;
	return params->Rpeak;

}

int searchback(int* peaks, int threshold2) {
	for (int i = 0; i < 1000; i++) {
		if (peaks[i] > threshold2) {
			return peaks[i];
		}
	}
	return peaks[0];
}

int average(int* array) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += array[i];
	}
	return (int) sum/8;
}

