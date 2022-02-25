/**
 * tcp_client.c
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "main.h"

int32_t gl_verbose_error = 1;

void seed_random()
{
  time_t cur_time;
  time(&cur_time);
  srand((uint32_t)(cur_time + getpid()));
}

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

  // setsockopt() to allow reuse.
  // 
  int32_t reuse_enable = 1;
  setsockopt(rtn_local_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_enable, sizeof(reuse_enable));
  setsockopt(rtn_local_fd, SOL_SOCKET, SO_REUSEPORT, &reuse_enable, sizeof(reuse_enable));

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

int32_t spawn_serial
  ( uint64_t arg_num_spawn
  , const char* arg_local_ipv4
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  )
{
  // vars.
  uint64_t n_sp = 0;
  uint16_t local_port = 0;
  int32_t rtn_spawn_error = 0;
  struct sockaddr_in local_sa;
  struct sockaddr_in remote_sa;

  // spawn in serial.
  for (n_sp = 0; n_sp < arg_num_spawn; n_sp++)
  {
    rtn_spawn_error = spawn_and_connect(
      arg_local_ipv4, 10000 + n_sp, arg_remote_ipv4, arg_remote_port, &local_sa, &remote_sa);

    if (rtn_spawn_error < 0)
    {
      if (gl_verbose_error)
      {
        printf("spawn_and_connect: error '%d'\n", rtn_spawn_error);
      }
    }
  }

  // ret.
  return rtn_spawn_error;
}

int main(int argc, char** argv)
{
  // seed random.
  seed_random();

  struct sockaddr_in local_sa;
  struct sockaddr_in remote_sa;
  // int32_t local_fd = spawn_and_connect(
  //   "127.0.0.1", (rand() % 10000) + 10000, "127.0.0.1", 44444, &local_sa, &remote_sa);
  // int32_t local_fd = spawn_and_connect(
  //   "192.168.0.10", (rand() % 10000) + 10000, "157.240.28.35", 443, &local_sa, &remote_sa);

  // useful IPs:
  // 127.0.0.1:44444 is `tcp-server`
  // 157.240.28.35:443 is `facebook.com`
  // 
  // spawn_serial(10, "127.0.0.1", "127.0.0.1", 44444);
  spawn_serial(100, "192.168.0.10", "157.240.28.35", 443);

  // ret.
  return 0;
}

