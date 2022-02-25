#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

/**
 * seed_random
 *
 * @summary Function to wrap srand(), to ensure that it receives a different
 * value on every invocation of this program.
 *
 */
void seed_random();

/**
 * spawn_and_connect
 * 
 * @summary
 *   Spawns a TCP client, and binds the client to 'arg_local_ipv4':'arg_local_port'.
 *   Connects the TCP client to the remote endpoint 'arg_remote_ipv4':'arg_remote_port'.
 *
 * @return
 *   The 'struct sockaddr_in' of the local TCP client [return parameter]
 *   The 'struct sockaddr_in' of the remote TCP client [return parameter]
 *   The socket file descriptor of the local TCP client [return value]
 *
 */
int32_t spawn_and_connect
  ( const char* arg_local_ipv4
  , uint16_t arg_local_port
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  , struct sockaddr_in* rtn_local_sa
  , struct sockaddr_in* rtn_remote_sa
  );

/**
 * spawn_and_connect_lock
 *
 * @summary
 *   `spawn_and_connect`, but places a lock around socket calls.
 *
 * @return
 *   ref. `spawn_and_connect`.
 *
 */
int32_t spawn_and_connect_lock
  ( const char* arg_local_ipv4
  , uint16_t arg_local_port
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  , struct sockaddr_in* rtn_local_sa
  , struct sockaddr_in* rtn_remote_sa
  );

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
 * spawn_and_connect_args
 * @summary
 *   Represents wrapper for arguments to `spawn_and_connect`.
 */
typedef struct
{
  const char*        _local_ipv4;
  uint16_t           _local_port;
  const char*        _remote_ipv4;
  uint16_t           _remote_port;
  struct sockaddr_in _local_sa;
  struct sockaddr_in _remote_sa;
} spawn_and_connect_args;

/**
 * spawn_and_connect_cb
 * 
 * @summary
 *   Callback wrapper for `spawn_and_connect`,
 *     to expose as void*(*func_name_here)(void*) for pthread.
 *
 * @return
 *   NULL, but provides printout for errors in `spawn_and_connect`.
 *
 */
void* spawn_and_connect_cb(void*);

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

#endif // MAIN_H

