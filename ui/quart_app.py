import asyncio
from quart import Quart
from services.listeners.telemetry_listener import TelemetryListener
from services.network_constants import QUART_WEBSOCKET_PORT
from services.websocket.telemetry_websocket import TelemetryWebSocket

quart_app = Quart(__name__)

telemetry_ws = TelemetryWebSocket(quart_app)

@quart_app.before_serving
async def startup() -> None:
    asyncio.create_task(telemetry_ws.broadcast_updates())
    telem_listener = TelemetryListener()
    telem_listener.start()

if __name__ == "__main__":
    quart_app.run(port=QUART_WEBSOCKET_PORT, debug=True)
