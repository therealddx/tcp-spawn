#ifndef UTIL_H
#define UTIL_H

#include "stdinclude.h"

/**
 * seed_random
 *
 * @summary Function to wrap srand(), to ensure that it receives a different
 * value on every invocation of this program.
 *
 */
void seed_random();

#endif // UTIL_H

