import asyncio
from quart import Quart
from services.listeners.camera_listener import CameraListener
from services.listeners.log_listener import LogListener
from services.listeners.telemetry_listener import TelemetryListener
from services.network_constants import QUART_WEB_PORT
from services.websocket.camera_websocket import CameraWebSocket
from services.websocket.logs_websocket import LogsWebSocket
from services.websocket.telemetry_websocket import TelemetryWebSocket

quart_app = Quart(__name__)

camera_ws = CameraWebSocket(quart_app)
log_ws = LogsWebSocket(quart_app)
telemetry_ws = TelemetryWebSocket(quart_app)

@quart_app.before_serving
async def startup() -> None:
    asyncio.create_task(camera_ws.broadcast_updates())
    camera_listener = CameraListener()
    camera_listener.start()
    
    asyncio.create_task(log_ws.broadcast_updates())
    log_listener = LogListener()
    log_listener.start()

    asyncio.create_task(telemetry_ws.broadcast_updates())
    telem_listener = TelemetryListener()
    telem_listener.start()

if __name__ == "__main__":
    quart_app.run(port=QUART_WEB_PORT, debug=True)
