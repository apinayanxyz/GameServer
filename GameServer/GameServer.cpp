#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

using namespace std;
int globalGold = 50;
DWORD WINAPI ServerThreadData(LPVOID lpParam) {
	SOCKET socket = (SOCKET)lpParam;
	int byteCount;
	while (1) {
		cout << "Global gold is " << globalGold << endl;
		Sleep(1000);
		if (socket != INVALID_SOCKET) {
			byteCount = send(socket, (char*)&globalGold, sizeof(globalGold), 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Server send error " << WSAGetLastError() << endl;
				closesocket(socket);
				return -1;
			}
			else {
				cout << "Server: sent " << byteCount << " bytes" << endl;
			}
			char receiveBuffer[200] = "";
			int byteCount = recv(socket, (char*)&globalGold, sizeof(globalGold), 0);
			if (byteCount < 0) {
				printf("Client: error %ld.\n", WSAGetLastError());
				return 0;
			}
			else {
				printf("Received data : % s  \n", receiveBuffer);
			}

		}
	}
	closesocket(socket);
	return 0;
}


DWORD WINAPI ServerThread(LPVOID lpParam) {
	SOCKET socket = (SOCKET)lpParam;
	int byteCount;
	char buffer[200] = "";
	while (1) {
		if (socket != INVALID_SOCKET) {
			byteCount = recv(socket, buffer, 200, 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Connection closed" << endl;
				return -1;
			}
			if (byteCount == 0) {
				shutdown(socket, SD_SEND);
				closesocket(socket);
				socket = INVALID_SOCKET;
				return -1;
			}
			else {
				cout << "From Client : " << buffer << endl;
				char sendBuffer[200] = "Message Received";
				byteCount = send(socket, sendBuffer, 200, 0);
				if (byteCount == SOCKET_ERROR) {
					cout << "Server send error " << WSAGetLastError() << endl;
					closesocket(socket);
					return -1;
				}
				else {
					cout << "Server: sent " << byteCount << endl;
				}
			}
		}
	}
	closesocket(socket);
	return 0;
}

int main(int argc, char* argv[]) {
	SOCKADDR_STORAGE from;
	int retval, fromlen, socket_type;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];


	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		cout << "The Winsock dll not found!" << endl;
		return 0;
	}
	else {
		cout << "The Winsock dll found!" << endl;
		cout << "The status: " << wsaData.szSystemStatus << endl;
	}

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket() is OK!" << endl;
	}


	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		cout << "bind() failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		cout << "bind() is OK!" << endl;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR)
		cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
	else
		cout << "listen() is OK, I'm waiting for connections..." << endl;
	DWORD threadId;
	while (1) {
		acceptSocket = accept(serverSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			cout << stderr << " accept failed: " << WSAGetLastError() << endl;
			return -1;
		}
		else {
			cout << "Server: Client Connected!" << endl;
			CreateThread(NULL, 0, ServerThreadData, (LPVOID)acceptSocket, 0, &threadId);
		}
	}
	system("pause");
	WSACleanup();


	return 0;
}