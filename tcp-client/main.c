#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "util.h"
#include "spawn.h"
#include "spawn_many.h"

int32_t gl_verbose_error = 1;

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

