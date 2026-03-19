#include "srv_sig_cond.h"

float srvSigCondSaturate(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float srvSigCondMedian(float value, float *buf, int *head, int *count, int size) {
    buf[*head] = value;
    *head = (*head + 1) % size;
    if (*count < size) (*count)++;

    // Copy to temp array for sorting
    float temp[MEDIAN_SIZE];
    for (int i = 0; i < *count; i++) {
        temp[i] = buf[i];
    }

    // Simple insertion sort
    for (int i = 1; i < *count; i++) {
        float key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    return temp[*count / 2]; // Return median
}

float srvSigCondIIR(float median, float *wma, bool *ready, int alpha) {
    if (!*ready) {
        *wma = median;
        *ready = true;
    } else {
        *wma = (alpha * median + (100 - alpha) * *wma) / 100;
    }
    return *wma;
}