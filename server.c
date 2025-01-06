#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdint.h>

    #include "validation.h"
    #include "security.h"
    #include "loading.h"
    #include <winsock2.h>
    #include "dbms.h"
    #include "input_from_user.h"

#define SERVER_PORT 8080

// Function declarations
void handle_client(SOCKET clientSocket);
void start_server();

int main() {
    start_server();
    return 0;
}

void handle_client(SOCKET clientSocket) {
    User user;
    int bytesReceived = recv(clientSocket, (char*)&user, sizeof(User), 0);

    if (bytesReceived <= 0) {
        printf("Failed to receive user data.\n");
        return;
    }

    switch(user.userRequest)
    {

    case REGISTER:
        printf("Received registration request for user: %s\n", user.userName);

        if (find_username(user.userName)) {
            // User already exists
            const char* response = "UserExists";
            send(clientSocket, response, strlen(response), 0);
            printf("User '%s' already exists.\n", user.userName);
            return;
        } else {
            // Save user to file
            if (store_user(&user)) {
                const char* response = "RegisterOK";
                send(clientSocket, response, strlen(response), 0);
                printf("User '%s' registered successfully.\n", user.userName);
                return;
            } else {
                const char* response = "RegisterError";
                send(clientSocket, response, strlen(response), 0);
                printf("Failed to save user '%s'.\n", user.userName);
                return;
            }
        }
        break;

    case LOGIN:
        if(find_user(user.userName,user.password))
        {
            const char* response = "LogInOK";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s logged in successfully.\n", user.userName);

        }
        else
        {
            const char* response = "LogInError";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s failed to log in due to wrong details!\n");
        }
        break;

    case DELETE_ACCOUNT:
        if(Delete_User(&user))
        {
            const char* response = "DeleteOk";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s deletion successfully!\n");
        }
        else
        {
            const char* response = "DeleteError";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s deletion failed!\n");
        }
        break;

    case CHANGE_PASSWORD:
        if(Update_Password(&user))
        {
            const char* response = "ChangeOk";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s update password successfully!\n", user.userName);
        }
        else
        {
            const char* response = "ChangeError";
            send(clientSocket, response, strlen(response), 0);
            printf("User %s update password failed!\n", user.userName);
        }
        break;

    default:
        {
            printf("unkown request!\n");
            break;
        }
    }
}

void start_server() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock initialization failed.\n");
        return;
    }

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    // Accept and handle client connections
    while ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) != INVALID_SOCKET) {
        printf("Client connected!\n");
        handle_client(clientSocket);
        closesocket(clientSocket);
    }

    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed.\n");
    }

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();
}

