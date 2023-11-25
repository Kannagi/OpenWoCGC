#include "system/crashlib.h"

//MATCH GCN
unsigned long timeGetTime(void) {
	return 0;
    //return OSGetTick(); //SDK GCN
}

//MATCH GCN
void DBTimerStart(int index) {
    unsigned long time;

    time = timeGetTime();
    DBTimers[index].start = time;
    return;
}

//MATCH GCN
void DBTimerEnd(int index) {
    unsigned long avg;
    unsigned long time;
    unsigned long elapsed;

    time = timeGetTime();
    avg = DBTimers[index].average;
    elapsed = DBTimers[index].elapsed + (time - DBTimers[index].start);
    DBTimers[index].stop = time;
    DBTimers[index].elapsed = elapsed;
    DBTimers[index].average = avg + elapsed >> 1;
    DBTimers[0].elapsed = 0xa4cb8;
    return;
}

//MATCH GCN
void DBTimerReset(void) {
    s32 i;

    for (i = 0; i < 0x28; i++) {
        DBTimers[i].elapsed = 0;
        DBTimers[i].stop = 0;
        DBTimers[i].start = 0;
    }
    return;
}
