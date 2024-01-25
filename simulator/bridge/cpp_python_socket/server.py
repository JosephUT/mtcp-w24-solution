import socket
from socket import socket as Socket

class CppReadSocket:
    """
    Python socket to read from a MessageClientSocket.
    """
    def __init__(self, connection: Socket):
        self.connection = connection
        self.messages = []
        self.back_done = False

    def receive(self) -> str:
        message = None
        if (self.back_done) or (len(self.messages)) > 1:
            message = self.messages[0]
            self.messages.pop(0)
        else:
            messages_new = self.connection.recv(4096).decode().split('\n')
            back_done_new = messages_new[-1] == ''
            messages_new = messages_new[:-1]
            print('remove back ', messages_new)

            if not self.back_done and len(self.messages) != 0:
                self.messages[-1] + messages_new[0]
                messages_new.pop(0)
                print('check back done ', messages_new)

            self.messages += messages_new
            self.back_done = back_done_new
            message = self.messages[0]
            self.messages.pop(0)
        if self.messages == []: self.back_done = False
        print('final state', self.messages)
        return message

server = socket.socket()         
server.bind(('', 3000))          
server.listen(1)                 
connection, address = server.accept()
cpp_read_socket = CppReadSocket(connection)
while True:
    print(cpp_read_socket.receive())
