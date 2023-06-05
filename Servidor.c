//Librerías a utilizar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_PORT 12345 //Define el puerto a utilizar

//Inicializa Winsock
int main() {
    //time_t tiempo = time(0);
    //struct tm *tlocal = localtime(&tiempo);
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error al inicializar Winsock.\n");
        return 1;
    }
 
    //Crea el socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Error al crear el socket del servidor: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //Configuración de la dirección del Servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    //Vincula el socket con la direccion del servidor
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Error al enlazar el socket del servidor: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    //Escucha si hay alguna conexion entrante
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Error al escuchar en el socket del servidor: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Esperando conexiones...\n");

    SOCKET client_socket;
    struct sockaddr_in client_address;
    int client_address_len = sizeof(client_address);

    char client_message[1024];
    char server_message[1024];

    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Error al aceptar la conexión del cliente: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("\n\t------------------------------------------------------\n");
    printf("\t|Conversacion Iniciada|\n");
    printf("\t------------------------------------------------------\n");
    printf("Cliente en linea.\n");
    printf("Hora de conexion: ");
    //printf("%H:%M:%S",tlocal);
    printf("\n\n");

    //Bucle para recibir y mandar mensajes
    int read_size;
    while (1) {
        memset(client_message, 0, sizeof(client_message));
        read_size = recv(client_socket, client_message, sizeof(client_message), 0);

        if (read_size > 0) {
            printf("Cliente: %s", client_message);

            if (strncmp(client_message, "adios", 5) == 0) {
                printf("Cerrando conexion del cliente.\n");
                break;
            }

            printf("Tu (Servidor): ");
            fgets(server_message, sizeof(server_message), stdin);
            send(client_socket, server_message, strlen(server_message), 0);

        } else if (read_size == 0) {
            printf("Cliente desconectado.\n");
            break;
        } else {
            printf("Error al recibir el mensaje del cliente: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(client_socket); //Cierra socket de Cliente
    closesocket(server_socket); //Cierra socket de Servidor
    WSACleanup(); //Finaliza el Winsock
    return 0;
}