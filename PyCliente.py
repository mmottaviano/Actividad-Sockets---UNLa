import socket

def menuOpciones():
    print("-------------------------------------")
    print("Opciones:")
    print("1-Generador de nombre de usuario")
    print("2-Generador de contrasena")
    print("3-Cerrar sesion")
    print("-------------------------------------")
    return input("Ingrese una opcion: ")

def main():
    server_ip = "127.0.0.1"
    port = 8080

    # Creo el socket TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    print("Iniciando Cliente...")
    try:
        # Conecta al servidor
        sock.connect((server_ip, port))
        print("Conexion exitosa!")
    except socket.error:
        print(f"Conexion fallida!")
        return

    while True:
        opcion = menuOpciones()

        # Envio la opción al servidor
        sock.send(opcion.encode('utf-8'))

        if opcion == '1' or opcion == '2':   
            longitud = int(input("Ingrese la longitud: "))
            # Envio la longitud al servidor
            sock.send(longitud.to_bytes(4, byteorder='little'))            

        # Recibo la respuesta del servidor
        respuesta = sock.recv(1024).decode('utf-8')
        print(f"Respuesta: {respuesta}\n")

        if opcion == '3':
            break
        else :
            input("Presione Enter para continuar...\n")


    sock.close()

if __name__ == "__main__":
    main()