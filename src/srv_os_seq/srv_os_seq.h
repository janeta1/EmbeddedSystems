#ifndef SRV_OS_SEQ_H
#define SRV_OS_SEQ_H

typedef struct {
    void (*taskFunc)(void);   // pointer to function
    int period;               // recurrence
    int offset;               // initial offset
    int counter;              // intern counter
} Task_t;

#define TASK1_PERIOD_MS 10
#define TASK2_PERIOD_MS 100
#define TASK3_PERIOD_MS 10000

void srvOsSeqInit();

#endif // SRV_OS_SEQ_H