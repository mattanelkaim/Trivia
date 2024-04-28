from socket import socket, AF_INET, SOCK_STREAM

SERVER_PORT = 7777
SERVER_IP = 'localhost'
ERROR_SOCKET_VALUE = None
BUFFER_SIZE = 1024
BUFFER_TEMPLATE = "{code}{msg_len}{msg}"
MAX_MESSAGE_LENGTH_SIZE = 9999
BYTES_RESERVED_FOR_MSG_LENGTH = 4

LOGIN_TEST_MESSAGE = """00037{username: "user1", password: "1234"}"""
SIGNUP_TEST_MESSAGE = """10062{username: "user1", password: "1234", mail: "user1@gmail.com"}"""


# This is unused at the moment but i think it will be useful in upcoming versions
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
        if input("PLease choose which test to run:"
                 "0: login test"
                 "not 0: signup test") == '0':
            print(f"Testing login - sending \"{LOGIN_TEST_MESSAGE}\"...")
            server_sock.send(bytes(LOGIN_TEST_MESSAGE))
        else:
            print(f"Testing signup - sending \"{SIGNUP_TEST_MESSAGE}\"...")
            server_sock.send(bytes(SIGNUP_TEST_MESSAGE))

        print(f"answer was: \"{server_sock.recv(BUFFER_SIZE).decode()}\"")
    except Exception as e:
        print("Connection was unexpectedly cut-off"
              f"exception caught is: {e}")


if __name__ == '__main__':
    main()
