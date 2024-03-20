/*
EL4236 Perancangan Perangkat Lunak Jaringan 2023/2024
*Hari dan Tanggal : Rabu, 20 Maret 2024
*Nama (NIM) 1     : Karma Kunga (13220028)
*Nama (NIM) 2     : Bostang Palaguna (13220055)
*Nama (NIM) 3     : Emmanuella Pramudita Rumanti (13220031)
*Nama File        : server.c
*Deskripsi        : sourcecode server untuk program komunikasi TCP socket
*Programmer       : Bostang Palaguna
*Referensi        : https://github.com/bostang/ServerClientSocket/blob/main/server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./lib/art.c"

#define PORT 4567

int PORT_CLIENT1;
int PORT_CLIENT2;

// INISIASI FUNGSI
void response(char*buff, char* input);

// int flag_kirim = 1;

// ALGORITMA UTAMA
int main()
{
    cetakBanner();

    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", PORT);

    if(listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }else
    {
        printf("[-]Error in binding.\n");
    }

    int clientCount = 0; // Variable to count the number of connected clients

    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        clientCount++;
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if(clientCount == 1) {
            printf("Assigning client1 to port %d\n", ntohs(newAddr.sin_port));
            PORT_CLIENT1 = ntohs(newAddr.sin_port);
            // Handle client1
        } else if(clientCount == 2) {
            printf("Assigning client2 to port %d\n", ntohs(newAddr.sin_port));
            PORT_CLIENT2 = ntohs(newAddr.sin_port);
            // Handle client2
        } else {
            printf("Maximum number of clients reached. Connection from %s:%d rejected.\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            close(newSocket);
            continue;
        }

        if((childpid = fork()) == 0){
            close(sockfd);

        while(1)
        {
            recv(newSocket, buffer, 1024, 0);
 
            if (strcmp(buffer,"selesai") == 0)
            {
                if (ntohs(newAddr.sin_port) == PORT_CLIENT1)
                {
                    printf("Client1 Memutus koneksi...\n");

                }
                else if (ntohs(newAddr.sin_port) == PORT_CLIENT2)
                {
                    printf("Client2 Memutus koneksi...\n");
                }
            }
            else
            {
                if (ntohs(newAddr.sin_port) == PORT_CLIENT1)
                {
                    printf("Client1 (%s:%d): %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);
                    // Process message from client1
                }
                else if (ntohs(newAddr.sin_port) == PORT_CLIENT2)
                {
                    printf("Client2 (%s:%d): %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);
                    // Process message from client2
                }
                else
                {
                    printf("Unknown client (%s:%d): %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);
                    // Handle unknown client or error condition
                }

                // send(newSocket, buffer, strlen(buffer), 0);

                response(buffer, buffer); // Menghasilkan respons berdasarkan input dari client
                send(newSocket, buffer, strlen(buffer), 0); // Mengirim respons ke client
                printf("Server: %s\n",buffer);
            }
            bzero(buffer, sizeof(buffer));
        }
    }

}

    close(newSocket);

    return 0;
}

void response(char*buff, char* input)
{
    if (strcmp(input,"nama1") == 0)
    {
        strcpy(buff,nama_developer1);
    }
    else if (strcmp(input,"nama2") == 0)
    {
        strcpy(buff,nama_developer2);
    }
    else if (strcmp(input,"nama3") == 0)
    {
        strcpy(buff,nama_developer3);
    }
    else if (strcmp(input,"NIM1") == 0)
    {
        strcpy(buff,NIM_developer1);
    }
    else if (strcmp(input,"NIM2") == 0)
    {
        strcpy(buff,NIM_developer2);
    }
    else if (strcmp(input,"NIM3") == 0)
    {
        strcpy(buff,NIM_developer3);
    }
        // selain isi buffer di atas, maka akan ditampilkan pesan error 
    else
    {
        strcpy(buff,"Perintah tidak diketahui");
    }
}