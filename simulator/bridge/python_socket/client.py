import socket             
 
ClientSocket = socket.socket()           
ClientSocket.connect(('127.0.0.1', 3000)) 
while True:
    message = input('enter message: ')
    ClientSocket.send(message.encode())
    if message == 'close': break
ClientSocket.close()