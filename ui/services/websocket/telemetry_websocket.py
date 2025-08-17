from quart import Quart
from services.databus.telemetry_databus import telem_databus
from services.network_constants import TELEMETRY_WEBSOCKET_PATH
from services.websocket.websocket import WebSocket

class TelemetryWebSocket(WebSocket):
    def __init__(self, quart_app: Quart):
        super().__init__(quart_app, TELEMETRY_WEBSOCKET_PATH, telem_databus)
