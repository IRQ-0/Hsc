#ifndef CPULOAD_H
#define CPULOAD_H

#include <windows.h>
#include <iostream>
#include <math.h>

static float calculateCPULoad(unsigned long long, unsigned long long);
static unsigned long long fileTimeToInt64(const FILETIME);
int roundFloat(float);
int getCpuLoad(void);

#endif
