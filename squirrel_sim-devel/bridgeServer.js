const net = require('net');
const WebSocket = require('ws');

let gui_port = 13349; // change in code as well to match at socket = new WebSocket("ws://localhost:13349"); line 
let controller_port = 13348; // change in code as well to match

const SocketController = new WebSocket.Server({ port: gui_port });

let mainClient = null;
SocketController.on('connection', (ws) => {
  mainClient = ws;
  console.log('main.js client connected!');

  ws.on('message', function (message) {
    console.log('Received: %s', message);
  });

  ws.on('close', function () {
    console.log('main.js WebSocket connection closed');
    mainClient = null;
  });
});

// TCP for Controller 
const server = net.createServer((socket) => {
  console.log('controller.cpp client connected!');

  socket.on('data', (data) => {
    console.log('Received: ' + data);
    if (wsClient) {
      console.log('Sending message to main.js: ' + data);
      wsClient.send(data);
    }
  });
  
  socket.on('end', () => {
    console.log('controller.cpp disconnected');
  });

}).on('error', (err) => {
  console.error(err);
});

server.listen({ port: controller_port, host: 'localhost' }, () => {
  console.log('TCP Server is running on port 13349.');
});
