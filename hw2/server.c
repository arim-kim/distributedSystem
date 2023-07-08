#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PAIRS 10000 // The maximum number of key-value pairs
#define MAX_STRLEN 256 // The maximum length of key and value strings

/* KVS structure */
typedef struct {
    char key[MAX_STRLEN];
    char value[MAX_STRLEN];
} KeyValuePair;

KeyValuePair kv_store[MAX_PAIRS];
int kv_count = 0;


// Function to put a key-value pair into the dictionary
void put(const char *key, const char *value) {
    int i;
    for (i = 0; i < kv_count; i++) {
        if (strcmp(kv_store[i].key, key) == 0) {
            strcpy(kv_store[i].value, value);
            return;
        }
    }

    if (kv_count < MAX_PAIRS) {
        strcpy(kv_store[kv_count].key, key);
        strcpy(kv_store[kv_count].value, value);
        kv_count++;
    }
}

// Function to get the value for a key from the dictionary
const char *get(const char *key) {
    int i;
    for (i = 0; i < kv_count; i++) {
        if (strcmp(kv_store[i].key, key) == 0) {
            return kv_store[i].value;
        }
    }
    return NULL;
}



int main(int argc, char *argv[]) {
	if ( argc < 2 ){
	 printf("Input : %s port number\n", argv[0]);
	 return 1;
	}

	int SERVER_PORT = atoi(argv[1]);

	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create listen socket\n");
		exit(1);
	}

	if ((bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) < 0) {
		printf("Could not bind socket\n");
		exit(1);
	}


	struct sockaddr_in cli_addr;
  int cli_addr_len = sizeof(cli_addr);

	int  maxlen = 1024;
	int n = 0;
	char RecvBuffer[maxlen];
  char SendBuffer[maxlen];

  while (1) {
      n = recvfrom(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&cli_addr,(socklen_t*) &cli_addr_len);
      if (n > 0) {
    	/* Please add your codes here!!! */
		RecvBuffer[n] = '\0';
		printf("%s", RecvBuffer); 
	
		char *temp = strtok(RecvBuffer," "); 

		if(strcmp(temp,"get") == 0) { // get
			char g_key[MAX_STRLEN];
			int count = 0; 
			temp = strtok(NULL," ");
			strcpy(g_key, temp);
			while(temp!=NULL) {
				temp = strtok(NULL," ");
				count+=1; 
	 		}

	 if(count != 1) {
		printf("%s\n",  "Invalid command");
		strcpy(RecvBuffer,  "Invalid command");

	 }else {
			for(int i = 0; g_key[i]!=0; i++) {
			if(g_key[i] == '\n') {
				g_key[i] = 0;
				break; 
			}
		}
		
		if(get(g_key) == NULL) {
			printf("%s\n", "Key not found, GET failed");
			strcpy(RecvBuffer,  "Key not found");
		}else {
			printf("%s\n", "GET is done");
			char g_value[MAX_STRLEN];
			strcpy(g_value,  get(g_key));
			for(int i = 0; g_value[i]!=0; i++) {
				if(g_value[i] == '\n') {
					g_value[i] = 0;
					break; 
				}
			}
			strcpy(RecvBuffer, g_value);
		}
	 }
	
	}else if(strcmp(temp,"put") == 0){ // put 
				
		char p_key[MAX_STRLEN];
		char p_value[MAX_STRLEN];

		int count = 0; 
		temp = strtok(NULL," ");
		strcpy(p_key, temp);

		if(temp != NULL) {
			temp = strtok(NULL," ");
			if(temp != NULL){
				strcpy(p_value, temp);
			}
			while(temp!=NULL) {
				temp = strtok(NULL," ");
				count+=1; 
			}
		}

	 if(count != 1) {
		strcpy(RecvBuffer,  "Invalid command");
		printf("%s\n",  "Invalid command");

	 }else {
		for(int i = 0; p_key[i]!=0; i++) {
			if(p_key[i] == '\n') {
				p_key[i] = 0;
				break; 
			}
		}

		put(p_key, p_value); 
		printf("%s\n",  "PUT is done");
		strcpy(RecvBuffer,  "OK");
	 }

	}else {
		strcpy(RecvBuffer, "Invalid command");
		printf("%s\n",  "Invalid command");
	}

	sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr*)&(cli_addr), sizeof(cli_addr)); 
	}
    
  }
	close(sock);

	return 0;
}
