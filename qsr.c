#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakDetection(int* MWI, int* peaks, QRS_params *params) {


	if ((MWI[0] <= MWI[1]) && (MWI[1] <= MWI[2]) && (MWI[2] >= MWI[3]) && (MWI[3] >= MWI[4])) {
		params->peak = MWI[2];
		shuffleArray(peaks, 10000, params->peak);
		//printf("%i \n", params->peak);
		if (params->peak > params->THRESHOLD1) {
			params->RR = params->SincePeak;
			params->SincePeak = 0;
			//printf("RR: %i \n", params->RR);

			if ((params->RR_LOW < params->RR) && (params->RR < params->RR_HIGH)) {
				params->Rpeak = params->peak;
				params->SPKF = (int) (0.125*params->peak + 0.875*params->SPKF);

				shuffleArray(params->RecentRR_OK, 8, params->RR);
				shuffleArray(params->RecentRR, 8, params->RR);


				params->RR_Average2 = average(params->RecentRR_OK);
				params->RR_Average1 = average(params->RecentRR);

				params->RR_LOW = (int) (0.92*params->RR_Average2);
				params->RR_HIGH = (int) (1.16*params->RR_Average2);
				params->RR_MISS = (int) (1.66*params->RR_Average2);
				params->THRESHOLD1 = (int) (params->NPKF  + (0.25*(params->SPKF-params->NPKF)));
				params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
				//printf("Count: %i, A1: %i, Peak: %i, NPKF: %i, SPKF: %i, LOW: %i, HIGH: %i, MISS: %i, T1: %i, T2: %i\n",params->count,params->RR_Average1, params->Rpeak, params->NPKF, params->SPKF, params->RR_LOW, params->RR_HIGH, params->RR_MISS, params->THRESHOLD1, params->THRESHOLD2);
				printf("%i %i \n", params->count, params->Rpeak);
			} else {
				if (params->RR > params->RR_MISS) {
					int peak2 = searchback(peaks, params->THRESHOLD2);
					params->Rpeak = peak2;
					params->SPKF = (int) ((0.25*peak2) +  (0.75*params->SPKF));
					shuffleArray(params->RecentRR, 8, params->RR);
					params->RR_Average1 = average(params->RecentRR);
					params->RR_LOW = (int) (0.92*params->RR_Average1);
					params->RR_HIGH = (int)  (1.16*params->RR_Average1);
					params->RR_MISS = (int) (1.66*params->RR_Average1);
					params->THRESHOLD1 = (int) (params->NPKF + (0.25*(params->SPKF-params->NPKF)));
					params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
					//printf("Count: %i, A1: %i, Peak: %i, SPKF: %i, LOW: %i, HIGH: %i, MISS: %i, T1: %i, T2: %i\n",params->count, params->RR_Average1, params->Rpeak, params->SPKF, params->RR_LOW, params->RR_HIGH, params->RR_MISS, params->THRESHOLD1, params->THRESHOLD2);
					//printf("%i %i \n", params->count, params->Rpeak);
					//printf("MISS");
				}
			}

		} else {
			params->NPKF = (int) (0.125*params->peak + 0.875*params->NPKF);
			//printf("NPKF: %i\n", params->NPKF);
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
	int back;
	for (int i = 0; i < 10000; i++) {
		if (peaks[i] > back) {
			back = peaks[i];
		}
	}
	return back;
}

int average(int* array) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += array[i];
	}
	return (int) (sum/8);
}

