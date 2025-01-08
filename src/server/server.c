#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int start_server()
{
  WSADATA wsaData;
  SOCKET server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};
  char *http_response = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 13\r\n\r\n"
                        "Hello, World!";

  // Initialize Winsock
  if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
      printf("WSAStartup failed\n");
      return 1;
    }

  // Create socket
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
      printf("Socket creation failed: %d\n", WSAGetLastError());
      WSACleanup();
      return 1;
    }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to address and port
  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
      printf("Bind failed: %d\n", WSAGetLastError());
      closesocket(server_fd);
      WSACleanup();
      return 1;
    }

  // Listen for connections
  if(listen(server_fd, SOMAXCONN) == SOCKET_ERROR)
    {
      printf("Listen failed: %d\n", WSAGetLastError());
      closesocket(server_fd);
      WSACleanup();
      return 1;
    }

  printf("Server listening on port %d...\n", PORT);

  while(1)
    {
      // Accept incoming connection
      if((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET)
        {
          printf("Accept failed: %d\n", WSAGetLastError());
          continue;
        }

      // Read incoming request
      recv(new_socket, buffer, BUFFER_SIZE, 0);
      printf("Received request:\n%s\n", buffer);

      // Send response
      send(new_socket, http_response, (int)strlen(http_response), 0);
      printf("Response sent\n");

      closesocket(new_socket);
    }

  closesocket(server_fd);
  WSACleanup();
  return 0;
}