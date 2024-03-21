/*
EL4236 Perancangan Perangkat Lunak Jaringan 2023/2024
*Hari dan Tanggal : Rabu, 20 Maret 2024
*Nama (NIM) 1     : Karma Kunga (13220028)
*Nama (NIM) 2     : Bostang Palaguna (13220055)
*Nama (NIM) 3     : Emmanuella Pramudita Rumanti (13220031)
*Nama File        : client.c
*Deskripsi        : sourcecode client untuk program komunikasi TCP socket
*Programmer       : Bostang Palaguna
*Referensi        : https://github.com/nikhilroxtomar/Multiple-Client-Server-Program-in-C-using-fork
*/

// INCLUDE LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>     // Linux-only, use Winsock2 for Windows
#include <sys/types.h>
#include <netinet/in.h>     // Linux-only
#include <arpa/inet.h>      // Linux-only

// DEKLARASI KONSTANTA
#define PORT 4567

int main()
{
    // setup client
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1)
	{
		// menerima pesan dari client
		printf("Client: ");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		// jika client ingin dis-connect dari server
		if(strcmp(buffer, "selesai") == 0)
		{
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		bzero(buffer, sizeof(buffer));

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: %s\n", buffer);
		}
	}

	return 0;
}
