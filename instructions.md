# Running MTCP

1. Build controller and system
2. run `pip install websockets`, or modify the docker file to do so
3. run the system target
4. run `python3 -m http.server 8000`
5. Open `localhost:8000` in your browser. You may have to redirect to the `squirrel_sim-devel` directory
6. the system target should now be connected to the monitor
7. run the controller target
8. Notice the bug, help Jake fix it