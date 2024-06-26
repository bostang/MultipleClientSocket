/*
EL4236 Perancangan Perangkat Lunak Jaringan 2023/2024
*Hari dan Tanggal : Rabu, 20 Maret 2024
*Nama (NIM) 1     : Karma Kunga (13220028)
*Nama (NIM) 2     : Bostang Palaguna (13220055)
*Nama (NIM) 3     : Emmanuella Pramudita Rumanti (13220031)
*Nama File        : client_cpp.cpp
*Deskripsi        : Source code client untuk program komunikasi TCP socket (C++ version)
*Programmer       : Emmanuella Rumanti
*Referensi        : https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/

// Compile: g++ client_cpp.cpp -o client_cpp -lws2_32

#define DEFAULT_ADDR "10.6.101.147"
#define DEFAULT_PORT "4568"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#define DEFAULT_BUFLEN 2048

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")s
#pragma comment (lib, "AdvApi32.lib")


int __cdecl main(int argc, char **argv) {

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("[+]Client Socket is created.\n");

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_ADDR, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    printf("[+]Connected to Server.\n");

    while (1) {
        // menerima pesan dari client
        printf("Client: ");
        scanf("%s", &sendbuf);

        iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
        //printf("bytes: %d", iResult);

        if (strcmp(sendbuf, "selesai") == 0) {
            printf("[-] Disconnected from server.\n");
            closesocket(ConnectSocket);
            WSACleanup();
            exit(1);
        }

        if (recv(ConnectSocket, recvbuf, 1024, 0) < 0) {
            printf("[-]Error in receiving data.\n");
        } else {
            printf("\nServer: %s\n", recvbuf);
        }
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
