#ifndef FILTERS_H
#define FILTERS_H

// Feel free to change return statement and arguments
int lowPassFilter(int data,int *pua,int *pfa);

// Implement additional filters
int highPassFilter(int data,int *pua,int *pfa);

int derivative(int data,int *pua);

int squaring(int data);

int MWI(int data, int *pua);
#endif // FILTERS_H
