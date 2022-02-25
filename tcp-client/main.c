/**
 * tcp_client.c
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int32_t gl_verbose_error = 1;

/**
 * spawn_and_connect
 * 
 * @summary
 *   Spawns a TCP client, and binds the client to 'arg_local_ipv4:arg_local_port'.
 *   Connects the TCP client to the remote endpoint 'arg_remote_ipv4:arg_remote_port'.
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
  )
{
  // arg check.
  //
  if ( (rtn_local_sa == NULL) || (rtn_remote_sa == NULL) ) { return -1; }

  // socket().
  // 
  int32_t rtn_local_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (rtn_local_fd < 0)
  {
    if (gl_verbose_error)
    {
      printf("socket: error '%d', errno '%d' ('%s')\n"
      , rtn_local_fd
      , errno
      , strerror(errno)
      );
    }
    return rtn_local_fd;
  }

  // bind() to the local.
  //
  rtn_local_sa->sin_family = AF_INET;
  inet_pton( AF_INET, arg_local_ipv4, &(rtn_local_sa->sin_addr.s_addr) );
  rtn_local_sa->sin_port = htons(arg_local_port);

  int32_t error_bind = bind(rtn_local_fd, (struct sockaddr*)(rtn_local_sa), sizeof(*rtn_local_sa));
  if (error_bind < 0)
  {
    if (gl_verbose_error)
    {
      printf("bind: error '%d', errno '%d' ('%s')\n"
      , error_bind
      , errno
      , strerror(errno)
      );
    }
    return error_bind;
  }

  // connect() to the remote.
  //
  rtn_remote_sa->sin_family = AF_INET;
  inet_pton( AF_INET, arg_remote_ipv4, &(rtn_remote_sa->sin_addr.s_addr) );
  rtn_remote_sa->sin_port = htons(arg_remote_port);

  int32_t error_connect = connect(rtn_local_fd, (struct sockaddr*)(rtn_remote_sa), sizeof(*rtn_remote_sa));
  if (error_connect < 0)
  {
    if (gl_verbose_error)
    {
      printf("connect: error '%d', errno '%d' ('%s')\n"
      , error_connect
      , errno
      , strerror(errno)
      );
    }
    return error_connect;
  }

  // ret.
  return rtn_local_fd;
}

/**
 * seed_random
 *
 * @summary Function to wrap srand(), to ensure that it receives a different
 * value on every invocation of this program.
 *
 */
void seed_random()
{
  time_t cur_time;
  time(&cur_time);
  srand((uint32_t)(cur_time + getpid()));
}

int main(int argc, char** argv)
{
  seed_random();

  // bind at 127.0.0.1:<rand()>, and connect to 127.0.0.1:44444.
  struct sockaddr_in local_sa;
  struct sockaddr_in remote_sa;
  int32_t local_fd = spawn_and_connect(
    "127.0.0.1", (rand() % 10000) + 10000, "127.0.0.1", 44444, &local_sa, &remote_sa);

  // ret.
  return 0;
}

