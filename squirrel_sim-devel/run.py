import asyncio
import json
from websockets.server import serve


async def echo(websocket):
    # load in a sample pose2D
    x, y, theta = 2, 3, 0.5
    while True:
        pose2D = {"x": x, "y": y, "theta": theta}
        await websocket.send(json.dumps({"type": "pose2D", "pos": pose2D}))
        x += 0.3
        theta += 0.5
        await asyncio.sleep(2)


async def squirrel_sim(websocket):
    async for message in websocket:
        pose2d = json.loads(message)
        print(f"Message received: {message}")
        await websocket.send(json.dumps({"type": "pose2D", "pos": pose2d}))


async def main():
    async with serve(squirrel_sim, "localhost", 8009):
        await asyncio.Future()  # run forever

asyncio.run(main())
