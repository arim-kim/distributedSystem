#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	// if ( argc < 2 ){
	//  printf("Input : %s port number\n", argv[0]);
	//  return 1;
	// }

	int sock, sock2, sock3;
	int count = 0; 


	int SERVER_PORT = 5001;
    /* localhost에서 통신할 것이므로 서버 ip주소도 그냥 localhost */
	const char* server_name = "localhost"; // 127.0.0.1
	struct sockaddr_in srv_addr; // Create socket structure
	memset(&srv_addr, 0, sizeof(srv_addr)); // Initialize memory space with zeros
	srv_addr.sin_family = AF_INET; // IPv4
	srv_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_name, &srv_addr.sin_addr);  // Convert IP addr. to binary


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create socket\n");
		exit(1);
	}

	int SERVER_PORT2 = 5002;
    /* localhost에서 통신할 것이므로 서버 ip주소도 그냥 localhost */
	const char* server_name2 = "localhost"; // 127.0.0.1
	struct sockaddr_in srv_addr2; // Create socket structure
	memset(&srv_addr2, 0, sizeof(srv_addr2)); // Initialize memory space with zeros
	srv_addr2.sin_family = AF_INET; // IPv4
	srv_addr2.sin_port = htons(SERVER_PORT2);
	inet_pton(AF_INET, server_name2, &srv_addr2.sin_addr);  // Convert IP addr. to binary


	if ((sock2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create socket\n");
		exit(1);
	}

	int SERVER_PORT3 = 5003;
    /* localhost에서 통신할 것이므로 서버 ip주소도 그냥 localhost */
	const char* server_name3 = "localhost"; // 127.0.0.1
	struct sockaddr_in srv_addr3; // Create socket structure
	memset(&srv_addr3, 0, sizeof(srv_addr3)); // Initialize memory space with zeros
	srv_addr3.sin_family = AF_INET; // IPv4
	srv_addr3.sin_port = htons(SERVER_PORT3);
	inet_pton(AF_INET, server_name3, &srv_addr3.sin_addr);  // Convert IP addr. to binary


	if ((sock3 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create socket\n");
		exit(1);
	}

	int n,m,o = 0;
	int maxlen = 1024;
	char RecvBuffer[maxlen],RecvBuffer2[maxlen],RecvBuffer3[maxlen];
    char SendBuffer[maxlen];
	int ack[] = {0,0,0}; 
	char ackNum[] = ""; 

  struct sockaddr_in cli_addr;
  int cli_addr_len = sizeof(cli_addr);

  while (1) {
      if (fgets(SendBuffer, maxlen, stdin) != NULL) {
          int bytes_sent = sendto(sock, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
		  int bytes_sent2 = sendto(sock2, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&srv_addr2, sizeof(srv_addr2));
          int bytes_sent3 = sendto(sock3, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&srv_addr3, sizeof(srv_addr3));

          memset(SendBuffer, 0, maxlen);
      }

      n = recvfrom(sock, RecvBuffer, sizeof(RecvBuffer) - 1, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
      m = recvfrom(sock2, RecvBuffer2, sizeof(RecvBuffer2) - 1, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
      o = recvfrom(sock3, RecvBuffer3, sizeof(RecvBuffer3) - 1, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);

      if (n > 0) {
		count ++; 
          RecvBuffer[n] = '\0';
          printf("%s\n", RecvBuffer);
		  int a_n = atoi(RecvBuffer); 
		  ack[a_n-1]++; 
          memset(RecvBuffer, 0, maxlen);
      }

	  if (m > 0) {
		count ++; 
          RecvBuffer2[m] = '\0';
          printf("%s\n", RecvBuffer2);
		  int a_n = atoi(RecvBuffer2); 
		  ack[a_n-1]++; 
          memset(RecvBuffer2, 0, maxlen);
      }

	  if (o > 0) {
		  count ++; 
          RecvBuffer3[o] = '\0';
          printf("%s\n", RecvBuffer3);
		  int a_n = atoi(RecvBuffer3); 
		  ack[a_n-1]++;  
          memset(RecvBuffer3, 0, maxlen);
      }

	  if(count == 3) {
		int flag = 0; // FALSE
		count = 0; // init
		for(int i = 0; i < 3; i++){
			if(ack[i] > 1) {
				flag = 1; 	
				break; 	
			}
		}

		if(flag == 1) {
			printf("%s\n", "TRUE");
		}else {
			printf("%s\n", "FALSE");

		}

		for(int i = 0; i < 3; i++){ // init
			ack[i] = 0;
		}

	  }
  }

	close(sock);
	close(sock2);
	close(sock3);

	return 0;
}
