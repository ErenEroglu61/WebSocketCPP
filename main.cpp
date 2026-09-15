#include <iostream>
#include <windows.h>
#include <winsock2.h>



int main(int argc, char** argv) {
	WORD version = MAKEWORD(2,2);
	WSADATA wsaData;
	int af,type,protocol;
	
	af = AF_INET;
	type = SOCK_STREAM;
	protocol = IPPROTO_TCP;
	
	int returned_value = WSAStartup(version,&wsaData);
	
	if (returned_value != 0) {
		printf("Winsock initialization  failed. \n");
		return -1;
	}	
	
	else {
		printf("Winsock initialized successfully.\n");
	}
	
	SOCKET webSocket = socket(af,type,protocol); // int af, int type, int protocol
	
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
	
	int bindValue = bind(webSocket,(SOCKADDR *) &serverAdress,sizeof(serverAdress));
	
	if (bindValue == SOCKET_ERROR) {
		printf("Binding failed = %d\n", WSAGetLastError() );
		closesocket(webSocket);
		WSACleanup();
		return -1;
	}
	
	else {
		printf("Binding successful !\n");
	}
	
	int listenValue = listen(webSocket,SOMAXCONN);
	
	if (listenValue == SOCKET_ERROR) {
		printf("Listening failed = %d\n", WSAGetLastError());
		return -1;
	}
	
	else {
		printf("Server is listening!\n");
	}
	
	int addressLength = sizeof(serverAdress);
	sockaddr_in clientAddress;
	
	
	SOCKET AcceptSocket = accept(webSocket,(SOCKADDR *) &clientAddress, &addressLength);
	
	if (AcceptSocket == INVALID_SOCKET) {
		printf("Accept socket failed = %d\n", WSAGetLastError());
		return -1;
	}
	
	else {
		printf("Server is accepted!\n");
	}
	
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int recvValue; 


	do {
	
		recvValue = recv(AcceptSocket,recvbuf,recvbuflen,0);
		
		if (recvValue > 0) {
			printf("Bytes Received: %d \n", recvValue);
		}
		else if (recvValue == 0){
			printf("Connection closed!\n");
		}
		else {
			printf("recv failed = %d\n", WSAGetLastError());
		}
		
	
	}	while( recvValue > 0);
	
	
	WSACleanup();
	return 0;
}
