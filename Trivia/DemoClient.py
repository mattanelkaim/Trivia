from socket import socket, AF_INET, SOCK_STREAM
from typing import Callable

SERVER_PORT = 7777
SERVER_IP = 'localhost'
ERROR_SOCKET_VALUE = None
BUFFER_SIZE = 1024
BUFFER_TEMPLATE = "{code}{msg_len}{msg}"
LOGIN_JSON_TEMPLATE = '{{"username": "{}", "password": "{}", "email": "{}"}}'
SIGNUP_JSON_TEMPLATE = '{{"username": "{}", "password": "{}"}}'
MAX_MESSAGE_LENGTH_SIZE = 9999
BYTES_RESERVED_FOR_MSG_LENGTH = 4
LOGIN_CODE = 0
SIGNUP_CODE = 1

USER1: {str: str} = {'username': "test_user_1", 'password': '1234', 'email': 'a1@b.c'}
USER2: {str: str} = {'username': "test_user_2", 'password': '1234', 'email': 'a2@b.c'}
USER3: {str: str} = {'username': "test_user_3", 'password': '1234', 'email': 'a3@b.c'}


def signup(user: {str: str}, sock: socket) -> None:
    json = LOGIN_JSON_TEMPLATE.format(user['username'], user['password'], user['email'])
    msg = BUFFER_TEMPLATE.format(code=SIGNUP_CODE, msg_len=get_padded_number(len(json)), msg=json)
    print(f'[CLIENT] {msg}')
    sock.send(msg.encode())
    print(f'[SERVER] {sock.recv(BUFFER_SIZE).decode()}')


def login(user: {str: str}, sock: socket) -> None:
    json = SIGNUP_JSON_TEMPLATE.format(user['username'], user['password'])
    msg = BUFFER_TEMPLATE.format(code=LOGIN_CODE, msg_len=get_padded_number(len(json)), msg=json)
    print(f'[CLIENT] {msg}')
    sock.send(msg.encode())
    print(f'[SERVER] {sock.recv(BUFFER_SIZE).decode()}')


# test functions
def test_unsigned_login(sock: socket) -> None:
    # trying to log in with a non-existent user
    print("Trying to login with an user who's not signed in:")
    login(USER1, sock)


def test_double_signup(sock: socket) -> None:
    for _ in range(2):
        print("Trying to signup with the same user twice:")
        signup(USER2, sock)
    # TODO: signout


def test_double_login(sock: socket) -> None:
    # signing up normally first
    signup(USER3, sock)

    # trying to login twice
    for _ in range(2):
        login(USER3, sock)

    # TODO: logout and signout


TESTS: {int: Callable[..., None]} = {1: test_unsigned_login, 2: test_double_signup, 3: test_double_login}


def get_padded_number(num: int) -> str:
    if num > MAX_MESSAGE_LENGTH_SIZE:
        raise ValueError

    return "0" * (BYTES_RESERVED_FOR_MSG_LENGTH - len(str(num))) + str(num)


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
        print("Server is currently closed, Please try again later.")
        exit(1)


def main():
    server_sock = create_tcp_socket()

    try:
        while True:
            TESTS[int(input("PLease choose which test to run:\n"
                            "1: try to log in with an unsigned user\n"
                            "2: try to sign up with a name that's already in the DB\n"
                            "3: try to log in twice\n"
                            "> "))](server_sock)

    except Exception as e:
        print("Connection was unexpectedly cut-off\n"
              "exception caught is:", e)


if __name__ == '__main__':
    main()
