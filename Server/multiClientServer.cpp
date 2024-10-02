#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>  // for cout/cerr
#include <vector>    // for storing client sockets
#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib") // Ensure linking with Winsock 2 library

using namespace std;

const int port = 55555;
const char ip[] = "127.0.0.1"; // for localhost
const int backlog = 5;         // maximum number of connection requests allowed
const int bufferSize = 1024;    // buffer size for receiving data

int main() {
    cout << "\n======== Server Socket =========\n";

    cout << "\n==== 1. Set up DLL ====\n";
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed.\n";
        return 1;
    }
    else{
        cout<<"The Winsock dll found!\n";
        cout<<"The status: "<<wsaData.szSystemStatus<<endl;
    }

    cout << "\n==== 2. Set up Server Socket ====\n";

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout<<"Error at Socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 1;
    }
    else{
        cout<<"socket() is OK!\n";
    }

    // Set server socket to allow multiple connections (reuse address)
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    cout << "\n==== 3. Bind Socket ====\n";

    // Define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    else{
        cout<<"bind() is OK!\n";
    }

    cout << "\n==== 4. Initiate Listen ====\n";

    if (listen(serverSocket, backlog) == SOCKET_ERROR) {
        cout<<"listen(): Error listening the socket"<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    else{
        cout<<"listen() is OK! I'm waiting for connections...\n";
        cout << "Server is listening on " << ip << ":" << port << "\n";
    }


    cout << "\n==== 5. Accept Connection ====\n";

    // Set of socket descriptors
    fd_set masterSet, readSet;
    FD_ZERO(&masterSet);
    FD_SET(serverSocket, &masterSet);
    int maxFd = serverSocket;

    vector<SOCKET> clientSockets;

    while (true) {
        readSet = masterSet;  // Copy the master set to readSet

        // Wait for activity on one of the sockets
        int activity = select(maxFd + 1, &readSet, NULL, NULL, NULL);

        if (activity < 0 && WSAGetLastError() != WSAEINTR) {
            cout << "select() error: " << WSAGetLastError() << "\n";
            break;
        }

        // Check if it's a new connection
        if (FD_ISSET(serverSocket, &readSet)) {
            // Accept new connection
            SOCKET newClient = accept(serverSocket, NULL, NULL);
            if (newClient == INVALID_SOCKET) {
                cout << "accept() failed: " << WSAGetLastError() << "\n";
            } else {
                // Add new client to the master set
                FD_SET(newClient, &masterSet);
                clientSockets.push_back(newClient);
                if (newClient > maxFd) maxFd = newClient;
                cout << "\nNew client connected: Socket FD = " << newClient << "\n\n";
            }
        }

        // Check all clients for incoming data
        for (auto it = clientSockets.begin(); it != clientSockets.end(); ) {
            SOCKET clientSocket = *it;
            if (FD_ISSET(clientSocket, &readSet)) {
                char buffer[bufferSize];
                int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);

                if (bytesReceived <= 0) {
                    // Connection closed or error occurred
                    if (strcmp(buffer, "exit") == 0) {
                        if(closesocket(clientSocket) == SOCKET_ERROR)
                            cout<<"Error closing connection with the client - Socket FD = "<<clientSocket<<" : "<<WSAGetLastError()<<endl;
                        else{
                            cout << "\nClient disconnected: Socket FD = " << clientSocket << "\n\n";
                        }
                    } else {
                        cout << "recv() failed: " << WSAGetLastError() << "\n";
                    }

                    closesocket(clientSocket);
                    FD_CLR(clientSocket, &masterSet);
                    it = clientSockets.erase(it);
                } else {
                    // Null-terminate the received data and print
                    buffer[bytesReceived] = '\0';
                    cout << "Message from client (FD " << clientSocket << "): " << buffer << "\n";

                    // Echo the message back to the client
                    send(clientSocket, buffer, bytesReceived, 0);
                    ++it;
                }
            } else {
                ++it;
            }
        }
        // if no active connection, then terminate the server socket
        if(clientSockets.empty()) break;
    }
    
    cout << "==== 7. Close the Socket ====\n";
    // CLosing the server socket
    if(closesocket(serverSocket) == SOCKET_ERROR){
        cout<<"Error closing socket: "<<WSAGetLastError()<<endl;
    }
    else{
        cout<<"Server: close() is OK!"<<endl;
        cout<<"\n=== Thank You! Visit again. ===\n\n";
    }
    WSACleanup();
    return 0;
}
