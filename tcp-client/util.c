#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

void seed_random()
{
  time_t cur_time;
  time(&cur_time);
  srand((uint32_t)(cur_time + getpid()));
}

