import asyncio
import json
import socket
from socket import socket as Socket
import websockets


class CppReadSocket:
    '''
    Python socket to read from a MessageClientSocket.
    '''
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
            messages_new = self.connection.recv(1024).decode().split('\n')
            back_done_new = messages_new[-1] == ''
            messages_new = messages_new[:-1]
            if not self.back_done and len(self.messages) != 0:
                self.messages[-1] + messages_new[0]
                messages_new.pop(0)
            self.messages += messages_new
            self.back_done = back_done_new
            message = self.messages[0]
            self.messages.pop(0)
        if self.messages == []: self.back_done = False
        return message


async def handler(websocket):
    # Wait for the js websocket to connect, then accept the connection from the system.
    connection, address = server.accept()
    cpp_read_socket = CppReadSocket(connection)
    while True:
        # Receive from the system, then send to the js websocket.
        message = cpp_read_socket.receive()
        print('received: ', message)
        try:
            message_dict = json.loads(message)
            if 'x' not in message_dict or \
                'y' not in message_dict or \
                'theta' not in message_dict:
                print('message does not contain all necessary keys')
                raise Exception()
            await websocket.send(json.dumps(message_dict))
        except:
            print('failed to decode')


async def main():
    async with websockets.serve(handler, '', 8001):
        await asyncio.Future()  # run forever


server = socket.socket()         
server.bind(('', 3000))          
server.listen(1)
asyncio.run(main())
