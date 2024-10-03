# Client-Server Model

## Overview

This project implements a basic client-server model in C++ using TCP sockets. It showcases two different communication setups:

1. **Two-way communication**: A chat-like application where a client and a server can both send and receive messages.
2. **One-way communication**: A server that can receive messages from multiple clients simultaneously, with each client unaware of the others.

## Files Explanation
**Client/baseClient.cpp**: Implements a client that communicates with `baseServer.cpp` in two-way communication.

**Client/client.cpp**: Implements a client that sends messages to `multiClientServer.cpp` in one-way communication.

**Server/baseServer.cpp**: Implements a server that communicates with `baseClient.cpp` in two-way communication.

**Server/multiClientServer.cpp**: Implements a server that handles multiple client connections.

## Communication Setups
1. **Two-Way Communication (Single Client only)**:
   - The server (`baseServer.cpp`) and client (`baseClient.cpp`) establish a connection and communicate in a back-and-forth manner, similar to a chat application.
   
2. **One-Way Communication (Multiple Clients)**:
   - The server (`multiClientServer.cpp`) accepts multiple clients (`client.cpp`), receiving messages from all of them. Clients are unaware of other connected clients, but the server keeps track of all active connections.

## Requirements

- Windows operating system (due to Winsock dependency)
- C++ compiler (`g++`, `cl`, etc.)
- Winsock2 library for socket programming

## How to Build and Run

Navigate to that file in the directory in terminal
```bash
  g++ <file_name>.cpp -o <file_name> -lWs2_32
  ./<file_name>
```
