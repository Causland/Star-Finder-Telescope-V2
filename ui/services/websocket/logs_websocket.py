from quart import Quart
from services.databus.log_databus import log_databus
from services.network_constants import LOGS_WEBSOCKET_PATH
from services.websocket.websocket import WebSocket

class LogsWebSocket(WebSocket):
    def __init__(self, quart_app: Quart):
        super().__init__(quart_app, LOGS_WEBSOCKET_PATH, log_databus)
