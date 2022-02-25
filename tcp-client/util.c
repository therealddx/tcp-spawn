/**
 * @file util.c
 * @author therealddx
 * @summary
 *   Implements util.h
 *
 */

#include "stdinclude.h"
#include "util.h"

int32_t gl_verbose_error = 1;

void seed_random()
{
  time_t cur_time;
  time(&cur_time);
  srand((uint32_t)(cur_time + getpid()));
}

