#include <winsock2.h>
#include <ws2tcpip.h>
#include "stdafx.h"
#include "iostream"

#pragma comment(lib, "Ws2_32.lib") // Ensure linking with Winsock 2 library

using namespace std;

int main() {
    cout << "\n======== Server Socket =========\n";

    cout << "\n==== 1. Set up DLL ====\n";
    
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0){
        cout<<"The Winsock dll not found!\n";
        return 0;
    }
    else{
        cout<<"The Winsock dll found!\n";
        cout<<"The status: "<<wsaData.szSystemStatus<<endl;
    }

    cout << "\n==== 2. Set up Server Socket ====\n";

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket==INVALID_SOCKET){
        cout<<"Error at Socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }
    else{
        cout<<"socket() is OK!\n";
    }

    cout << "\n==== 3. Bind Socket ====\n";
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
        cout<<"bind() failed: "<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else{
        cout<<"bind() is OK!\n";
    }

    cout << "\n==== 4. Initiate Listen ====\n";
    if(listen(serverSocket,1) == SOCKET_ERROR)
        cout<<"listen(): Error listening the socket"<<WSAGetLastError()<<endl;
    else   
        cout<<"listen() is OK! I'm waiting for connections...\n";

    cout << "\n==== 5. Accept Connection ====\n";
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET){
        cout<<"accept failed: "<<WSAGetLastError()<<endl;
        WSACleanup();
        return -1;
    }
    else{
        cout<<"Accepted connection"<<endl;
    }

    cout << "\n==== 6. Chat with the Client ====\n";
    cout << "(type 'exit' to quit)\n\n";

    char recvbuf[512];
    int recvbuflen = 512;
    string sendbuf;
    int bytesReceived;

    while(true) {
        bytesReceived = recv(acceptSocket, recvbuf, recvbuflen, 0);
        if(bytesReceived > 0) {
            recvbuf[bytesReceived] = '\0'; // Null-terminate received data
            cout << "Client: " << recvbuf << endl;

            if(strcmp(recvbuf, "exit") == 0) {
                cout << "\nClient has exited the chat." << endl;
                break;
            }

            cout << "Server: ";
            getline(cin, sendbuf);
            send(acceptSocket, sendbuf.c_str(), sendbuf.length(), 0);

            if(sendbuf == "exit") {
                cout << "\nServer exiting the chat." << endl;
                break;
            }
        }
    }

    cout << "\n==== 7. Close the Socket ====\n";

    if(closesocket(acceptSocket) == SOCKET_ERROR)
        cout<<"Error closing connection with the client socket: "<<WSAGetLastError()<<endl;
    else{
        cout<<"accpetSocket: close() is OK!"<<endl;
    }
    if(closesocket(serverSocket) == SOCKET_ERROR)
        cout<<"Error closing socket: "<<WSAGetLastError()<<endl;
    else{
        cout<<"Server: close() is OK!"<<endl;
        cout<<"\n=== Thank You! Visit again. ===\n";
    }

    // system("pause");
    cout<<endl;
    WSACleanup();

    return 0;
}
