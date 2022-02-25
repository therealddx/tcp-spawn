/**
 * tcp_client.c
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
  srand(getpid());

  // socket().
  // 
  int32_t tcp_client_fd  = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_client_fd < 0)
  {
    printf("tcp_client: socket: error '%d'\n", tcp_client_fd);
  }
  printf("tcp_client: socket: ok\n");

  // fill out 'struct sockaddr_in'.
  //
  struct sockaddr_in tcp_client_sa;
  tcp_client_sa.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &tcp_client_sa.sin_addr.s_addr);
  tcp_client_sa.sin_port = htons((rand() % 10000) + 10000);

  // bind().
  //
  int32_t error_bind = bind(tcp_client_fd, (struct sockaddr*)(&tcp_client_sa), sizeof(tcp_client_sa));
  if (error_bind < 0)
  {
    printf("tcp_client: bind: error '%d'\n", error_bind);
    return error_bind;
  }
  printf("tcp_client: bind: ok\n");

  // connect().
  //
  struct sockaddr_in tcp_server_sa;
  tcp_server_sa.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &tcp_server_sa.sin_addr.s_addr);
  tcp_server_sa.sin_port = htons(44444);

  int32_t error_connect = connect(tcp_client_fd, (struct sockaddr*)(&tcp_server_sa), sizeof(tcp_server_sa));
  if (error_connect < 0)
  {
    printf("tcp_client: connect: error '%d'\n", error_connect);
    return error_connect;
  } 
  printf("tcp_client: connect: ok\n");

  // ret.
  return 0;
}

