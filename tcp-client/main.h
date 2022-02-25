#ifndef MAIN_H
#define MAIN_H

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
 * spawn_serial
 *
 * @summary
 *   Calls `spawn_and_connect` for 'arg_num_spawn' TCP clients, in serial.
 *   Each TCP client is bound to 'arg_local_ipv4' on a random port,
 *     and connects to 'arg_remote_ipv4':'arg_remote_port'.
 *   Returns early if any `spawn_and_connect` fails.
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

#endif // MAIN_H

