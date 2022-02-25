#include "spawn.h"

extern int32_t gl_verbose_error;
static pthread_mutex_t gl_lock_connect;

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

int32_t spawn_and_connect_lock
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

  // lock connect().
  pthread_mutex_lock(&gl_lock_connect);

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

  // unlock connect().
  pthread_mutex_unlock(&gl_lock_connect);

  // ret.
  return rtn_local_fd;
}

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

