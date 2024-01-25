import socket           
 
ServerSocket = socket.socket()         
ServerSocket.bind(('', 3000))          
ServerSocket.listen(5)                 
ConnectionSocket, address = ServerSocket.accept() 
while True:
    message = ConnectionSocket.recv(1024).decode()
    if message == 'close': break 
    print(message)
ConnectionSocket.close()
ServerSocket.close()