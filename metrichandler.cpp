#include "metrichandler.h"

double findMin(double* arr, int size){
    double min = arr[0];
    for (int i = 1; i < size; i++){
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

double findMax(double* arr, int size){
    double max = arr[0];
    for (int i = 1; i < size; i++){
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}
