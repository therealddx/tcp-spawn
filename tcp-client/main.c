/**
 * tcp_client.c
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "main.h"

int32_t gl_verbose_error = 1;


void* spawn_and_connect_cb(void* p_sac_args)
{
  // marshal args.
  spawn_and_connect_args* sac_args = (spawn_and_connect_args*)p_sac_args;

  // call.
  int32_t error_sac = spawn_and_connect_lock
    ( sac_args->_local_ipv4
    , sac_args->_local_port
    , sac_args->_remote_ipv4
    , sac_args->_remote_port
    , &( sac_args->_local_sa )
    , &( sac_args->_remote_sa )
    );

  if (error_sac < 0)
  {
    if (gl_verbose_error)
    {
      printf("spawn_and_connect: error '%d'\n", error_sac);
    }
  }

  // ret.
  return NULL;
}

int32_t spawn_parallel
  ( uint64_t arg_num_spawn
  , const char* arg_local_ipv4
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  , bool arg_should_join
  )
{
  // vars.
  uint64_t n_sp = 0;
  int32_t error_pcreate = 0;

  spawn_and_connect_args* sac_args =
    (spawn_and_connect_args*)malloc(sizeof(spawn_and_connect_args) * arg_num_spawn);

  pthread_t* all_pthreads = NULL;
  if (arg_should_join)
    { all_pthreads = (pthread_t*)malloc(sizeof(pthread_t) * arg_num_spawn); }

  // spawn in parallel.
  //
  for (n_sp = 0; n_sp < arg_num_spawn; n_sp++)
  {
    // vars, pthread.
    pthread_attr_t cur_pattr;
    pthread_attr_init(&cur_pattr); // default
    pthread_t cur_pthread;

    // vars, `spawn_and_connect`.
    sac_args[n_sp]._local_ipv4 = arg_local_ipv4;
    sac_args[n_sp]._remote_ipv4 = arg_remote_ipv4;
    sac_args[n_sp]._remote_port = arg_remote_port;
    sac_args[n_sp]._local_port = (n_sp % 40000) + 10000; // [0, 49999] => [10000, 59999]

    error_pcreate = pthread_create
      ( &cur_pthread
      , &cur_pattr
      , &spawn_and_connect_cb
      , (void*)( &(sac_args[n_sp]) )
      );

    if (error_pcreate < 0)
    {
      if (gl_verbose_error)
      {
        printf("pthread_create: error '%d'\n", error_pcreate);
      }
    }

    if (arg_should_join && all_pthreads != NULL)
      { all_pthreads[n_sp] = cur_pthread; }
  }

  // optionally, ensure all threads complete.
  //
  if (arg_should_join && all_pthreads != NULL)
  {
    for (n_sp = 0; n_sp < arg_num_spawn; n_sp++)
      { pthread_join(all_pthreads[n_sp], NULL); }
    free(all_pthreads);
  }

  // free the `spawn_and_connect` args.
  free(sac_args);

  // ret.
  return error_pcreate;
}

int main(int argc, char** argv)
{
  // seed random.
  seed_random();

  // struct sockaddr_in local_sa;
  // struct sockaddr_in remote_sa;
  // int32_t local_fd = spawn_and_connect(
  //   "127.0.0.1", (rand() % 10000) + 10000, "127.0.0.1", 44444, &local_sa, &remote_sa);
  // int32_t local_fd = spawn_and_connect(
  //   "192.168.0.10", (rand() % 10000) + 10000, "157.240.28.35", 443, &local_sa, &remote_sa);

  // useful IPs:
  // 127.0.0.1:44444 is `tcp-server`
  // 157.240.28.35:443 is `facebook.com`
  // 
  // spawn_serial(10, "127.0.0.1", "127.0.0.1", 44444);
  // spawn_serial(100, "192.168.0.10", "157.240.28.35", 443);

  // spawn_parallel(10000, "127.0.0.1", "127.0.0.1", 44444, true);
  spawn_parallel(1000, "192.168.0.10", "157.240.28.35", 443, true);

  // ret.
  return 0;
}

