//Librerias a utilizar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1" //Servidor IP, es localhost
#define SERVER_PORT 12345 //Puerto a usr

int main() {
    //time_t tiempo = time(0);
    //struct tm *tlocal = localtime(&tiempo);
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error al inicializar Winsock.\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        printf("Error al convertir la dirección IP.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Error al conectar al servidor: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("\n\t------------------------------------------------------\n");
    printf("\t|Conversacion Iniciada, escriba adios para salir|\n");
    printf("\t------------------------------------------------------\n");
    printf("Servidor en linea.\n");
    printf("Hora de conexion: ");
    //printf("%H:%M:%S",tlocal);
    printf("\n\n");

    char message[1024];
    while (1) {
        printf("Tu (Cliente): ");
        fgets(message, sizeof(message), stdin);
        send(sock, message, strlen(message), 0);

        if (strncmp(message, "adios", 5) == 0) {
            printf("Cerrando conexion.\n");
            break;
        }

        memset(message, 0, sizeof(message));
        recv(sock, message, sizeof(message), 0);
        printf("Servidor: %s\n", message);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}