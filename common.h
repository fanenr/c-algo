#ifndef TIME_H
#define TIME_H

#include <time.h>

#define TIME_ST() clock_t __tm_st__ = clock();
#define TIME_ED() clock_t __tm_ed__ = clock();
#define TIME_VAL() ((double)(__tm_ed__ - __tm_st__) / CLOCKS_PER_SEC)

#endif
