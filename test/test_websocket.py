import asyncio
import websockets

async def test_websocket():
    uri = "ws://koying.asuscomm.com:9002"  # 伺服器的 URI
    async with websockets.connect(uri) as websocket:
        # 發送多條消息給伺服器
        for i in range(5):
            message = f"Hello, server! Message {i+1}"
            await websocket.send(message)
            print(f"Message sent to server: {message}")

            # 接收伺服器的回覆
            response = await websocket.recv()
            print(f"Response from server: {response}")

# 執行測試
asyncio.get_event_loop().run_until_complete(test_websocket())