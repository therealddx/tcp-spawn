#ifndef SPAWN_MASS_H
#define SPAWN_MASS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "spawn.h"

/**
 * spawn_serial
 *
 * @summary
 *   Calls `spawn_and_connect` for 'arg_num_spawn' TCP clients, in serial.
 *   Each TCP client is bound to 'arg_local_ipv4' on an internally-calculated port,
 *     and connects to 'arg_remote_ipv4':'arg_remote_port'.
 *   Reports, but does not return-early, if any `spawn_and_connect` fails.
 *
 * @return
 *   The return value of the final `spawn_and_connect` call.
 */
int32_t spawn_serial
  ( uint64_t arg_num_spawn
  , const char* arg_local_ipv4
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  );

/**
 * spawn_parallel
 *
 * @summary
 *   Calls `spawn_and_connect` for 'arg_num_spawn' TCP clients, each on their own thread.
 *   Each TCP client is bound to 'arg_local_ipv4' on an internally-calculated port,
 *     and connects to 'arg_remote_ipv4':'arg_remote_port'.
 *   Reports, but does not return-early, if any `spawn_and_connect` fails.
 *
 * @return
 *   The return value of the final `pthread_create` call.
 *
 */
int32_t spawn_parallel
  ( uint64_t arg_num_spawn
  , const char* arg_local_ipv4
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  , bool arg_should_join
  );

#endif // SPAWN_MASS_H

