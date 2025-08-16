from quart import Quart, websocket
import asyncio
import json
from services.telemetry_bus import bus
from services.listeners.telemetry_listener import TelemetryListener
from services.websocket.telemetry_websocket import TelemetryWebSocket

quart_app = Quart(__name__)

telemetry_ws = TelemetryWebSocket(quart_app)

@quart_app.before_serving
async def startup():
    asyncio.create_task(telemetry_ws.broadcast_updates())
    telem_listener = TelemetryListener()
    telem_listener.start()

if __name__ == "__main__":
    quart_app.run(port=8051, debug=True)
