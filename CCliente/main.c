#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void menuOpciones();
void iniciarCliente(WSADATA *wsa, SOCKET *sock, struct sockaddr_in *server_addr);

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server_addr;

    char buffer[1024] = {0};
    char opcion;
    int longitud;
    int cantLeida;

    iniciarCliente(&wsa, &sock, &server_addr);
    system("PAUSE");

    while (1) {
        menuOpciones();

        scanf(" %c", &opcion);
        // Envio la opción
        send(sock, &opcion, sizeof(opcion), 0);

        if(opcion == '1' || opcion == '2'){
            system("CLS");
            printf("Ingrese la longitud: \n");
            scanf("%d", &longitud);

            // Envio la longitud
            send(sock, (char*)&longitud, sizeof(longitud), 0);
        }

        cantLeida = recv(sock, buffer, 1024, 0);
        buffer[cantLeida] = '\0';

        system("CLS");
        printf("Respuesta:\n%s\n\n", buffer);

        if (opcion == '3'){
            break;
        }else{
            system("PAUSE");
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

void menuOpciones() {
    system("CLS");
    printf("-------------------------------------\n");
    printf("Opciones:\n");
    printf("1-Generador de nombre de usuario\n");
    printf("2-Generador de contrasena\n");
    printf("3-Cerrar sesion\n");
    printf("-------------------------------------\n");
    printf("Ingrese una opcion:\n");
}

void iniciarCliente(WSADATA *wsa, SOCKET *sock, struct sockaddr_in *server_addr) {
    printf("Iniciando Cliente...\n");
    if (WSAStartup(MAKEWORD(2,2), wsa) != 0) {
        printf("Error en WSAStartup: %d\n", WSAGetLastError());
        exit(1);
    }

    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error en la creacion del socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(*sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        printf("Conexion fallida: %d\n", WSAGetLastError());
        closesocket(*sock);
        WSACleanup();
        exit(1);
    }
    printf("Conexion exitosa!\n\n");
}
