#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define MAX_PAIRS 10000 // The maximum number of key-value pairs
#define MAX_STRLEN 256  // The maximum length of key and value strings

/* KVS structure */
typedef struct
{
    char key[MAX_STRLEN];
    char value[MAX_STRLEN];
} KeyValuePair;

KeyValuePair kv_store[MAX_PAIRS];
int kv_count = 0;

// Function to put a key-value pair into the dictionary
void put(const char *key, const char *value)
{
    int i;
    for (i = 0; i < kv_count; i++)
    {
        if (strcmp(kv_store[i].key, key) == 0)
        {
            strcpy(kv_store[i].value, value);
            return;
        }
    }

    if (kv_count < MAX_PAIRS)
    {
        strcpy(kv_store[kv_count].key, key);
        strcpy(kv_store[kv_count].value, value);
        kv_count++;
    }
}

// Function to get the value for a key from the dictionary
const char *get(const char *key)
{
    int i;
    for (i = 0; i < kv_count; i++)
    {
        if (strcmp(kv_store[i].key, key) == 0)
        {
            return kv_store[i].value;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Input : %s port number\n", argv[0]);
        return 1;
    }

    int SERVER_PORT = atoi(argv[1]);
    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock, sock1, sock2;

    struct sockaddr_in cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    //----------------------
    const char *cli_name = "localhost"; // 127.0.0.1
    struct sockaddr_in c_addr;          // Create socket structure

    int maxlen = 1024;
    int n, m, o = 0;
    int ack_count = 0;
    char RecvBuffer[maxlen], RecvBuffer1[maxlen], RecvBuffer2[maxlen];
    char sentNum[] = "1"; 
    char okSign[] = "OK";
    char followerRecvBuffer[maxlen];
    char ackBuffer[maxlen];
    char SendBuffer[maxlen];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Could not create listen socket\n");
        exit(1);
    }

    if ((bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr))) < 0)
    {
        printf("Could not bind socket\n");
        exit(1);
    }

    while (1)
    {

        if(SERVER_PORT == 5001) {
            strcpy(sentNum, "1");
        }else {
            srand(time(NULL));
            int num = rand() % 3 + 1; // 1, 2, 3
            sprintf(sentNum,"%d",num);
        }

        n = recvfrom(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);
        
        if (n > 0)
        {
            /* Please add your codes here!!! */
            RecvBuffer[n] = '\0';
            // printf("%s", RecvBuffer);

            char *temp = strtok(RecvBuffer, " ");

            if ((strcmp(temp, "get") == 0))
            { 
                char g_key[MAX_STRLEN];
                int count = 0;
                temp = strtok(NULL, " ");
                strcpy(g_key, temp);
                while (temp != NULL)
                {
                    temp = strtok(NULL, " ");
                    count += 1;
                }

                if (count != 1)
                {
                    printf("%s\n", "Invalid command");
                    strcpy(RecvBuffer, "Invalid command");
                    sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
                }

                else
                {
                    for (int i = 0; g_key[i] != 0; i++)
                    {
                        if (g_key[i] == '\n')
                        {
                            g_key[i] = 0;
                            break;
                        }
                    }

                    // if (get(g_key) == NULL)
                    // {
                    //     // printf("%s\n", "Key not found, GET failed");
                    //     printf("%s\n", "Key not found");
                    //     strcpy(RecvBuffer, "Key not found");
                    // }
                    // else
                    // {
                    //     printf("%s\n", "GET is done");
                    //     char g_value[MAX_STRLEN];
                    //     strcpy(g_value, get(g_key));
                    //     for (int i = 0; g_value[i] != 0; i++)
                    //     {
                    //         if (g_value[i] == '\n')
                    //         {
                    //             g_value[i] = 0;
                    //             break;
                    //         }
                    //     }
                    //     strcpy(RecvBuffer, g_value);
                    // }
                    
                    strcpy(RecvBuffer, sentNum); 
                    printf("%s returned\n", sentNum);
                    sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
                }
            }
            // else if (strcmp(temp, "put") == 0)
            // { // put

            //     char p_key[MAX_STRLEN];
            //     char p_value[MAX_STRLEN];

            //     int count = 0;
            //     temp = strtok(NULL, " ");
            //     strcpy(p_key, temp);

            //     if (temp != NULL)
            //     {
            //         temp = strtok(NULL, " ");
            //         if (temp != NULL)
            //         {
            //             strcpy(p_value, temp);
            //         }
            //         while (temp != NULL)
            //         {
            //             temp = strtok(NULL, " ");
            //             count += 1;
            //         }
            //     }

            //     if (count != 1)
            //     {
            //         strcpy(RecvBuffer, "Invalid command");
            //         printf("%s\n", "Invalid command");
            //         sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
            //     }
            //     else
            //     {
            //         for (int i = 0; p_key[i] != 0; i++)
            //         {
            //             if (p_key[i] == '\n')
            //             {
            //                 p_key[i] = 0;
            //                 break;
            //             }
            //         }

            //         put(p_key, p_value); // put
            //         memset(SendBuffer, 0, maxlen);
            //         // strcat(SendBuffer, "to follower");
            //         strcat(SendBuffer, "put ");
            //         strcat(SendBuffer, p_key);
            //         strcat(SendBuffer, " ");
            //         strcat(SendBuffer, p_value);
            //         strcat(SendBuffer, "\0");

            //         // propagation  --------------------
            //         int follower1 = 5002;
            //         int follower2 = 5003;
            //         ack_count = 0;
            //         const char *server1_name = "localhost"; // 127.0.0.1
            //         struct sockaddr_in f1_addr;             // Create socket structure
            //         memset(&f1_addr, 0, sizeof(f1_addr));   // Initialize memory space with zeros
            //         f1_addr.sin_family = AF_INET;           // IPv4
            //         f1_addr.sin_port = htons(follower1);
            //         inet_pton(AF_INET, server1_name, &f1_addr.sin_addr);                                            // Convert IP addr. to binary
            //         sendto(sock1, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&f1_addr, sizeof(f1_addr)); // 보냄

            //         const char *server2_name = "localhost"; // 127.0.0.1
            //         struct sockaddr_in f2_addr;             // Create socket structure
            //         memset(&f2_addr, 0, sizeof(f2_addr));   // Initialize memory space with zeros
            //         f2_addr.sin_family = AF_INET;           // IPv4
            //         f2_addr.sin_port = htons(follower2);
            //         inet_pton(AF_INET, server2_name, &f2_addr.sin_addr);                                            // Convert IP addr. to binary
            //         sendto(sock2, SendBuffer, strlen(SendBuffer), 0, (struct sockaddr *)&f2_addr, sizeof(f2_addr)); // 보냄
            //         int f_addr_len = sizeof(f1_addr);
            //         bool flag = false;

            //         m = recvfrom(sock1, RecvBuffer1, sizeof(RecvBuffer1) - 1, 0, (struct sockaddr *)&f1_addr, (socklen_t *)&f_addr_len); // 받1
            //         o = recvfrom(sock2, RecvBuffer2, sizeof(RecvBuffer2) - 1, 0, (struct sockaddr *)&f2_addr, (socklen_t *)&f_addr_len); // 받2

            //         if (m > 0)
            //         {
            //             RecvBuffer1[m] = '\0';
            //             char *temp = strtok(RecvBuffer1, " ");

            //             if (strcmp(temp, "ACK") == 0)
            //             {
            //                 ack_count++;
            //                 if (ack_count == 2)
            //                 {
            //                     flag = true;
            //                 }
            //             }
            //         }

            //         if (o > 0)
            //         {

            //             RecvBuffer2[o] = '\0';
            //             char *temp = strtok(RecvBuffer2, " ");

            //             if (strcmp(temp, "ACK") == 0)
            //             {
            //                 ack_count++;
            //                 if (ack_count == 2)
            //                 {
            //                     flag = true;
            //                 }
            //             }
            //         }

            //         if (flag == true)
            //         {
            //             strcpy(RecvBuffer, "OK");
            //             strcat(RecvBuffer, "\0");
            //             sendto(sock, &RecvBuffer, sizeof(RecvBuffer) + 1, 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
            //             flag = false;
            //             printf("%s\n", "PUT is done");
            //         }
            //     }
            // }
            // else if (strcmp(temp, "ACK") != 0)
            // {
            //     strcpy(RecvBuffer, "Invalid command");
            //     printf("%s\n", "Invalid command");
            //     sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
            // }
        }
    }

    // while (1)
    // {
    //     n = recvfrom(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_addr_len);

    //     if (n > 0)
    //     {
    //         RecvBuffer[n] = '\0';
    //         printf("%s", RecvBuffer);
    //         char *temp1 = strtok(RecvBuffer, " ");

    //         if (strcmp(temp1, "put") == 0)
    //         {
    //             char p1_key[MAX_STRLEN];
    //             char p1_value[MAX_STRLEN];

    //             temp1 = strtok(NULL, " ");
    //             strcpy(p1_key, temp1);
    //             temp1 = strtok(NULL, " ");
    //             strcpy(p1_value, temp1);
    //             for (int i = 0; p1_key[i] != 0; i++)
    //             {
    //                 if (p1_key[i] == '\n')
    //                 {
    //                     p1_key[i] = 0;
    //                     break;
    //                 }
    //             }

    //             put(p1_key, p1_value); // put
    //             printf("%s\n", "PUT is done");
    //             strcpy(RecvBuffer, "ACK");
    //             sendto(sock, &RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr *)&(cli_addr), sizeof(cli_addr));
    //         }
    //     }
    // }

    close(sock);
    return 0;
}
