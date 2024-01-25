# Python to js with WebSockets
The web app is the client, the python is the server. Run the server, then server the webapp with python -m http.server.

# Python to Python with Sockets
Simple client server. Nothing interesting.

# C++ to Python with MessageSockets
Python socket that can read everything appropriately from a Connection socket. Basically just a Python implementation of the receiveMessage() call.

TODO: Combine these into one bridge class that has a Socket server and a WebSocket server and handles connections and shutdowns well.
TODO: Integrate the WebSocket into the frontend, and test robot movement.
TODO: Clean everything up and write more tests.
