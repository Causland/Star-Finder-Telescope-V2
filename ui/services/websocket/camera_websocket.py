from quart import Quart
from services.databus.camera_databus import camera_databus
from services.network_constants import CAMERA_WEBSOCKET_PATH
from services.websocket.websocket import WebSocket

class CameraWebSocket(WebSocket):
    def __init__(self, quart_app: Quart):
        super().__init__(quart_app, CAMERA_WEBSOCKET_PATH, camera_databus)