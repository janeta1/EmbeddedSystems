#ifndef SRV_SIG_COND_H
#define SRV_SIG_COND_H

#define SAT_MIN 50
#define SAT_MAX 950

#define MEDIAN_SIZE 5

#define IIR_ALPHA 30
#define RAMP_STEP 2

float srvSigCondSaturate(float value, float min, float max);
float srvSigCondMedian(float value, float *buf, int *head, int *count, int size);
float srvSigCondIIR(float median, float *wma, bool *ready, int alpha);
float srvSigCondRamp(float current, float target, float step);


#endif // SRV_SIG_COND_H