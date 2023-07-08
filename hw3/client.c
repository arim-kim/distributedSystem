#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	if ( argc < 2 ){
	 printf("Input : %s port number\n", argv[0]);
	 return 1;
	}

	int SERVER_PORT = atoi(argv[1]);
  /* localhost에서 통신할 것이므로 서버 ip주소도 그냥 localhost */
	const char* server_name = "localhost"; // 127.0.0.1
	struct sockaddr_in srv_addr; // Create socket structure
	memset(&srv_addr, 0, sizeof(srv_addr)); // Initialize memory space with zeros
	srv_addr.sin_family = AF_INET; // IPv4
	srv_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_name, &srv_addr.sin_addr);  // Convert IP addr. to binary


	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create socket\n");
		exit(1);
	}

	int n = 0;
	int maxlen = 1024;
	char RecvBuffer[maxlen];
  char SendBuffer[maxlen];

  struct sockaddr_in cli_addr;
  int cli_addr_len = sizeof(cli_addr);

  while (1) {
      if (fgets(SendBuffer, maxlen, stdin) != NULL) {
          int bytes_sent = sendto(sock, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
          memset(SendBuffer, 0, maxlen);
      }

      n = recvfrom(sock, RecvBuffer, sizeof(RecvBuffer) - 1, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);

      if (n > 0) {
          RecvBuffer[n] = '\0';
          printf("%s\n", RecvBuffer);
          memset(RecvBuffer, 0, maxlen);
      }
  }

	close(sock);
	return 0;
}
