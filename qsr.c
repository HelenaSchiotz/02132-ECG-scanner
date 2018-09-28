#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int peakDetection(int* MWI, int* peaks, QRS_params *params) {
	clock_t start, end;
	double cpu_time_used;
	start = clock();

	if ((MWI[0] <= MWI[1]) && (MWI[1] <= MWI[2]) && (MWI[2] >= MWI[3]) && (MWI[3] >= MWI[4]) && !(MWI[0] == MWI[1] ==MWI[2] == MWI[3] == MWI[4])) {
		params->peak = MWI[2];
		shuffleArray(params->RecentRR_RR, 10000, params->count);
		shuffleArray(peaks, 10000, params->peak);
		if (params->peak > params->THRESHOLD1) {
			params->RR = params->SincePeak;


			if ((params->RR_LOW < params->RR) && (params->RR < params->RR_HIGH)) {
				params->Rpeak = params->peak;
				params->SPKF = (int) (0.125*params->peak + 0.875*params->SPKF);
				params->SincePeak = 0;
				shuffleArray(params->RecentRR_OK, 8, params->RR);
				shuffleArray(params->RecentRR, 8, params->RR);
				shuffleArray(params->RecentRR_RR, 8, params->RR);


				params->RR_Average2 = average(params->RecentRR_OK);
				params->RR_Average1 = average(params->RecentRR);

				params->RR_LOW = (int) (0.92*params->RR_Average2);
				params->RR_HIGH = (int) (1.16*params->RR_Average2);
				params->RR_MISS = (int) (1.66*params->RR_Average2);
				params->THRESHOLD1 = (int) (params->NPKF  + (0.25*(params->SPKF-params->NPKF)));
				params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
				params->missed = 0;
				printf("Peak: %-6i Time: %-6i Pulse: %-6i ", params->Rpeak, params->count, ((params->RR*60)/250));
				if (params->missed >= 5) {
					printf("Status: Heart failure! \n");
				} else if (params->Rpeak < 2000) {
					printf("Status: Weak Heartbeat! \n");
				} else {
					printf("Status: All is fine \n");
				}

			} else {
				params->missed++;
				if (params->RR >= params->RR_MISS) {
					int peak2 = searchback(peaks, params->RecentRR_RR, params->THRESHOLD2);
					shuffleArray(params->RecentRR, 8, params->count-peak2);
					printf("Peak: %-6i Time: %-6i Pulse: %-6i ", MWI[params->count-peak2+2], peak2, (((params->count-peak2)*60)/250));
					params->SPKF = (int) ((0.25*MWI[params->count-peak2+2]) +  (0.75*params->SPKF));
					if (params->missed >= 5) {
						printf("Status: Heart failure! \n");
					} else if (params->Rpeak < 2000) {
						printf("Status: Weak Heartbeat! \n");
					} else {
						printf("Status: All is fine \n");
					}
					params->RR_Average1 = average(params->RecentRR);
					params->RR_LOW = (int) (0.92*params->RR_Average1);
					params->RR_HIGH = (int)  (1.16*params->RR_Average1);
					params->RR_MISS = (int) (1.66*params->RR_Average1);
					params->THRESHOLD1 = (int) (params->NPKF + (0.25*(params->SPKF-params->NPKF)));
					params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);

					params->SPKF = (int) ((0.25*params->Rpeak) +  (0.75*params->SPKF));
					shuffleArray(params->RecentRR, 8, params->RR);
					params->RR_Average1 = average(params->RecentRR);
					params->RR_LOW = (int) (0.92*params->RR_Average1);
					params->RR_HIGH = (int)  (1.16*params->RR_Average1);
					params->RR_MISS = (int) (1.66*params->RR_Average1);
					params->THRESHOLD1 = (int) (params->NPKF + (0.25*(params->SPKF-params->NPKF)));
					params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
					params->SincePeak = 0;

					printf("Peak: %-6i Time: %-6i Pulse: %-6i ",params->Rpeak, params->count, (((params->RR-(params->count-peak2))*60)/250));
					if (params->missed >= 5) {
						printf("Status: Heart failure! \n");
					} else if (params->Rpeak < 2000) {
						printf("Status: Weak Heartbeat! \n");
					} else {
						printf("Status: All is fine \n");
					}
				}

			}

		} else {
			params->NPKF = (int) (0.125*params->peak + 0.875*params->NPKF);
			params->THRESHOLD1 = (int) (params->NPKF + 0.25*(params->SPKF-params->NPKF));
			params->THRESHOLD2 = (int) (0.5*params->THRESHOLD1);
		}
	}


	params->SincePeak++;
	params->count++;

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	//printf("Time: %-10f \n", cpu_time_used);
	return params->Rpeak;

}

int searchback(int* peaks, int* time, int threshold2) {
	int back;
	for (int i = 1; i < 10000; i++) {
		if (peaks[i] > threshold2) {
			back = peaks[i];
			return time[i];
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


