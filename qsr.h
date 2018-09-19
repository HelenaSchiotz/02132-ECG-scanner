#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{ // Structure for QRS parameters
   int SPKF;
   int NPKF;
   int THRESHOLD1;
   int THRESHOLD2;
   int RR_Average1;
   int RR_Average2;
   int RR_LOW;
   int RR_HIGH;
   int RR_MISS;
   int* RecentRR_OK;
   int* RecentRR;
   int RR;
   int Rpeak;
   int peak;
   int SincePeak;
   int count;
   
} QRS_params;

// Feel free to change and add methods
int peakDetection(int* dataArray, int* peaks, QRS_params *params);

#endif // QSR_H
