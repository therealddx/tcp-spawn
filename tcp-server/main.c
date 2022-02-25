/**
 * @file main.c
 * @author therealddx
 * @summary
 *   Basic TCP listener implementation, to service client connections.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
  // vars.
  uint32_t n_clients = 0;

  // socket().
  // 
  int32_t tcp_listen_fd  = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_listen_fd < 0)
  {
    printf("tcp_server: socket: error '%d'\n", tcp_listen_fd);
  }
  printf("tcp_server: socket: ok\n");

  // fill out 'struct sockaddr_in'.
  //
  struct sockaddr_in tcp_listen_sa;
  tcp_listen_sa.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &tcp_listen_sa.sin_addr.s_addr);
  tcp_listen_sa.sin_port = htons(44444);

  // bind().
  //
  int32_t error_bind = bind(tcp_listen_fd, (struct sockaddr*)(&tcp_listen_sa), sizeof(tcp_listen_sa));
  if (error_bind < 0)
  {
    printf("tcp_server: bind: error '%d'\n", error_bind);
    return error_bind;
  }
  printf("tcp_server: bind: ok\n");

  // listen().
  //
  int32_t error_listen = listen(tcp_listen_fd, 16);
  if (error_listen < 0)
  {
    printf("tcp_server: listen: error '%d'\n", error_listen);
    return error_listen;
  }
  printf("tcp_server: listen: ok\n");

  while (1)
  {
    // accept().
    //
    struct sockaddr_in tcp_client_sa;
    socklen_t tcp_client_len = sizeof(tcp_client_sa);
    int32_t error_accept = accept(tcp_listen_fd, (struct sockaddr*)(&tcp_client_sa), &tcp_client_len);
    if (error_accept < 0)
    {
      printf("tcp_server: accept: error '%d\n", error_accept);
      return error_accept;
    }

    char client_ipv4[256];
    inet_ntop(AF_INET, &tcp_client_sa.sin_addr.s_addr, client_ipv4, sizeof(client_ipv4));
    printf("[%d]: tcp_server: accept: accepted client at %s:%d\n"
      , ++n_clients
      , client_ipv4
      , ntohs(tcp_client_sa.sin_port)
      );
  }

  // ret.
  return 0;
}
