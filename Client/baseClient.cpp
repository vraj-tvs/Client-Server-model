#include <winsock2.h>
#include <ws2tcpip.h>
#include "stdafx.h"
#include "iostream"

#pragma comment(lib, "Ws2_32.lib") // Ensure linking with Winsock 2 library

using namespace std;

int main() {
    cout << "\n======== Client Socket =========\n";

    cout << "\n==== 1. Set up DLL ====\n";
    
    SOCKET clientSocket;
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

    cout << "\n==== 2. Set up client Socket ====\n";

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket==INVALID_SOCKET){
        cout<<"Error at Socket(): "<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }
    else{
        cout<<"socket() is OK!\n";
    }

    cout << "\n==== 3. Connect to server Socket ====\n";
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _TEXT("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
        cout<<"Client: connect() - Failed to connect.\n";
        WSACleanup();
        return 0;
    }
    else{
        cout<<"Client: connect() is OK!\n";
        cout<<"Client: Can start sending and recieving data...\n"; 
    }

    cout << "\n==== 4. Chat with the server ====\n";
    cout << "(type 'exit' to quit)\n\n";

    char recvbuf[512];
    int recvbuflen = 512;
    string sendbuf;
    int bytesReceived;

    while(true) {
        cout << "Client: ";
        getline(cin, sendbuf);
        send(clientSocket, sendbuf.c_str(), sizeof(sendbuf), 0);

        if(sendbuf == "exit") {
            cout << "\nClient exiting the chat." << endl;
            break;
        }

        bytesReceived = recv(clientSocket, recvbuf, recvbuflen, 0);
        if(bytesReceived > 0) {
            recvbuf[bytesReceived] = '\0'; // Null-terminate received data
            cout << "Server: " << recvbuf << endl;

            if(strcmp(recvbuf, "exit") == 0) {
                cout << "\nServer has exited the chat." << endl;
                break;
            }
        }
    }


    cout << "\n==== 5. Close the socket ====\n";
    if(closesocket(clientSocket) == SOCKET_ERROR)
        cout<<"Error closing socket: "<<WSAGetLastError()<<endl;
    else{
        cout<<"Client: close() is OK!"<<endl;
        cout<<"\n=== Thank You! Visit again. ===\n";
    }

    // system("pause");
    cout<<endl;
    WSACleanup();

    return 0;
}