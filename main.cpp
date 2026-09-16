#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <string.h>
#define DEFAULT_BUFLEN 512

int main(int argc, char** argv) {


    WORD version = MAKEWORD(2, 2);
    WSADATA wsaData;
	const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 21\r\n"
    "\r\n"
    "Hello from C++ server!";
	
    int af, type, protocol;
	
    af = AF_INET;
    type = SOCK_STREAM;
    protocol = IPPROTO_TCP;

    int returned_value = WSAStartup(version, &wsaData);

    if (returned_value != 0) {
        printf("Winsock initialization failed.\n");
        return -1;
    }
    else {
        printf("Winsock initialized successfully.\n");
    }


    SOCKET webSocket = socket(af, type, protocol);

    if (webSocket == INVALID_SOCKET) {
        printf("Socket failed with error = %d\n", WSAGetLastError());

        WSACleanup();
        return -1;
    }
    else {
        printf("Socket function succeeded\n");
    }



    sockaddr_in serverAdress;

    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(8080);
    serverAdress.sin_addr.S_un.S_addr = INADDR_ANY;


    int bindValue = bind(
        webSocket,
        (SOCKADDR*)&serverAdress,
        sizeof(serverAdress)
    );

    if (bindValue == SOCKET_ERROR) {

        printf(
            "Binding failed = %d\n",
            WSAGetLastError()
        );

        closesocket(webSocket);
        WSACleanup();

        return -1;
    }
    else {
        printf("Binding successful!\n");
    }


    int listenValue = listen(
        webSocket,
        SOMAXCONN
    );

    if (listenValue == SOCKET_ERROR) {

        printf(
            "Listening failed = %d\n",
            WSAGetLastError()
        );

        closesocket(webSocket);
        WSACleanup();

        return -1;
    }
    else {
        printf("Server is listening!\n");
    }


    sockaddr_in clientAddress;

    int addressLength = sizeof(clientAddress);

    SOCKET AcceptSocket = accept(
        webSocket,
        (SOCKADDR*)&clientAddress,
        &addressLength
    );

    if (AcceptSocket == INVALID_SOCKET) {

        printf(
            "Accept socket failed = %d\n",
            WSAGetLastError()
        );

        closesocket(webSocket);
        WSACleanup();

        return -1;
    }
    else {
        printf("Server accepted a client!\n");
    }




    char recvbuf[DEFAULT_BUFLEN];

    // Reserve one byte for '\0'
    int recvbuflen = DEFAULT_BUFLEN - 1;

    int recvValue;


    do {

        recvValue = recv(
            AcceptSocket,
            recvbuf,
            recvbuflen,
            0
        );


        if (recvValue > 0) {

            // Null terminate the received data
            recvbuf[recvValue] = '\0';

            printf(
                "Bytes Received: %d\n",
                recvValue
            );

            printf(
                "Data Received:\n%s\n",
                recvbuf
            );
        }

        else if (recvValue == 0) {

            printf("Connection closed!\n");
        }

        else {

            printf(
                "recv failed = %d\n",
                WSAGetLastError()
            );
        }
        
        int sendValue =	send(AcceptSocket,response,(int)strlen(response),0);
	
		if (sendValue == SOCKET_ERROR) {
			printf("Send failed with error: %d\n", WSAGetLastError);
			closesocket(AcceptSocket);
			WSACleanup();
			return -1;
		}
		
		else {
			printf("Send successful!");
		}

    } while (recvValue > 0);

	
	
    closesocket(AcceptSocket);
    closesocket(webSocket);

    WSACleanup();

    return 0;
}
