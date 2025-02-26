#ifndef __FILTER_H
#define __FILTER_H

#include "at32f403a_407.h"              // Device header
#include "math.h"

#define PI 3.1415f  

void  LowPassFilter(float in, float *out, float dt, float fc);

#endif
