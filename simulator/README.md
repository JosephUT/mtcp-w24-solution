# Running the Simulator

1. Build with the CMakeLists.txt in this directory.
2. Run bridge.py.
3. Run python3 -m http.server to serve index.html, click on the link to go to localhost:8000.
4. Refresh the page so the websocket client connects to the bridge.
5. Run ./build/test_python_cpp_socket to test sending strings from C++ to the frontend.
6. Type in json encoded strings with x, y, and theta ({"x":1,"y":1,"theta":1}) and send them.
7. You should see the position of the block update in the webpage.