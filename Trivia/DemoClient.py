from typing import Union
from socket import socket, AF_INET, SOCK_STREAM

SERVER_PORT = 7777
SERVER_IP = 'localhost'
ERROR_SOCKET_VALUE = None
BUFFER_SIZE = 1024


def create_tcp_socket() -> socket:
    """
    This function creates a tcp conversation socket with the server
    :return: the socket
    :rtype: socket
    """

    try:
        server_sock = socket(AF_INET, SOCK_STREAM)
        server_address = (SERVER_IP, SERVER_PORT)
        server_sock.connect(server_address)
        return server_sock
    except ConnectionRefusedError:
        print("Server is currently closed.")
        return ERROR_SOCKET_VALUE  # error return value


def main():
    server_sock = create_tcp_socket()
    if server_sock == ERROR_SOCKET_VALUE:
        return 1

    server_sock.send(b'hello')
    answer = server_sock.recv(BUFFER_SIZE).decode()

    if answer != 'hello':
        print(f'Test failed!'
              f'answer was: {answer}')
    else:
        print('Test passed!')


if __name__ == '__main__':
    main()
