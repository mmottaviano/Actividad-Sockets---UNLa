#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define TAM 1024

void generadorNombre(char* nombre, int longitud);
void generadorContrasena(char* contrasena, int longitud);
void iniciarServidor(WSADATA *wsa, SOCKET *server_socket, struct sockaddr_in *server_addr);


int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);

    char respuesta[TAM];
    char opcion;
    int longitud;

    srand(time(NULL));

    iniciarServidor(&wsa, &server_socket, &server_addr);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) != INVALID_SOCKET) {

        printf("\nSe conecto un cliente.\n\n");

        while (1) {
            // Recibo opcion
            recv(client_socket, &opcion, sizeof(opcion), 0);

            printf("\nEl cliente ingreso la opcion: %c\n", opcion);

            if(opcion == '1' || opcion == '2'){
                recv(client_socket, &longitud, sizeof(longitud), 0);
                printf("El cliente ingreso la longitud: %d\n", longitud);
            }

            switch (opcion) {
                case '1':
                    generadorNombre(respuesta, longitud);
                    break;

                case '2':
                    generadorContrasena(respuesta, longitud);
                    break;

                case '3':
                    snprintf(respuesta, sizeof(respuesta), "Cerrando conexion.\n");
                    break;

                default:
                    snprintf(respuesta, sizeof(respuesta), "Error: opcion no valida\a\n");
                    break;
            }
            printf("Se envio al cliente la respuesta:\n %s\n", respuesta);
            send(client_socket, respuesta, strlen(respuesta), 0);

            if (opcion == '3') {
                break;
            }
        }
        closesocket(client_socket);
        printf("Se cerro la conexion con el cliente.\n");
        printf("Esperando siguiente cliente.");
    }

    WSACleanup();
    return 0;
}

void generadorNombre(char* nombre, int longitud) {

    if (5 <= longitud && longitud <= 15) {
        char vocales[] = "aeiou";
        char consonantes[] = "bcdfghjklmnpqrstvwxyz";

        int definicionAlternancia = rand() % 2;

        for (int i = 0; i < longitud; i++) {
            if (i % 2 == definicionAlternancia) {
                nombre[i] = vocales[rand() % 5];
            } else {
                nombre[i] = consonantes[rand() % 21];
            }
        }
        nombre[longitud] = '\0';

    } else {
        snprintf(nombre, TAM, "Error: el usuario debe ser de entre 5 y 15 caracteres.\a\n");
    }
}

void generadorContrasena(char* contrasena, int longitud) {

    if (8 <= longitud && longitud <= 50) {

        char caracteres[] = "0123456789aeioubcdfghjklmnpqrstvwxyzAEIOUBCDFGHJKLMNPQRSTVWXYZ";

        for (int i = 0; i < longitud; i++) {
            contrasena[i] = caracteres[rand() % 62];
        }
        contrasena[longitud] = '\0';

    } else {
        snprintf(contrasena, TAM, "Error: la contrasena debe ser de entre 8 y 50 caracteres.\a\n");
    }
}

void iniciarServidor(WSADATA *wsa, SOCKET *server_socket, struct sockaddr_in *server_addr) {
    printf("Iniciando Servidor...\n");
    if (WSAStartup(MAKEWORD(2,2), wsa) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        exit(1);
    }

    if ((*server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error en la creacion del socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);

    if (bind(*server_socket, (struct sockaddr *)server_addr, sizeof(*server_addr)) == SOCKET_ERROR) {
        printf("Error en bind: %d\n", WSAGetLastError());
        closesocket(*server_socket);
        WSACleanup();
        exit(1);
    }

    if (listen(*server_socket, 3) == SOCKET_ERROR) {
        printf("Error en listen: %d\n", WSAGetLastError());
        closesocket(*server_socket);
        WSACleanup();
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);
}
