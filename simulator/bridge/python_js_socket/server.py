#!/usr/bin/env python

import asyncio
import json
import websockets


async def handler(websocket):
    # The handler is only set up when the webpage client connects.
    while True:
        # Collect user input and send.
        value = input('Input message: ')
        event = {"message": value}
        await websocket.send(json.dumps(event))


async def main():
    async with websockets.serve(handler, "", 8001):
        await asyncio.Future()  # run forever


if __name__ == "__main__":
    asyncio.run(main())
