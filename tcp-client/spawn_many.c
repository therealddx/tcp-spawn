#include "spawn_many.h"

extern int32_t gl_verbose_error;

int32_t spawn_serial
  ( uint64_t arg_num_spawn
  , const char* arg_local_ipv4
  , const char* arg_remote_ipv4
  , uint16_t arg_remote_port
  )
{
  // vars.
  uint64_t n_sp = 0;
  int32_t rtn_spawn_error = 0;
  struct sockaddr_in local_sa;
  struct sockaddr_in remote_sa;

  // spawn in serial.
  for (n_sp = 0; n_sp < arg_num_spawn; n_sp++)
  {
    rtn_spawn_error = spawn_and_connect
      ( arg_local_ipv4
      , (n_sp % 50000) + 10000 // [0, 49999] => [10000, 59999]
      , arg_remote_ipv4
      , arg_remote_port
      , &local_sa
      , &remote_sa
      );

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

