#pragma once

#include "types.h"

#define assert(exp) if (!(exp)) { *(volatile int *)0 = 0; }
#define array_count(arr) (sizeof(arr) / sizeof(arr[0]))

struct Newstring { 
    u8 *data;
    size_t count;
};
