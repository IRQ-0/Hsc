#include "cpuload.h"

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) {
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;


    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    
    return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME ft) {
    return (((unsigned long long) (ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

inline int roundFloat(float in) {
	return in < 0 ? ceil(in - 0.5) : floor(in + 0.5);
}

int getCpuLoad(void) {
    FILETIME idleTime, kernelTime, userTime;
    
    return roundFloat((GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f) * 100);   
}
