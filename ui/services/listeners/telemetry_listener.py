from models.telemetry import unpack_telemetry
from services.network_constants import WIFI_USER_ADDR, WIFI_TELEM_PORT
from services.telemetry_bus import bus
from services.listeners.udp_listener import UDPListener

class TelemetryListener(UDPListener):
    def __init__(self):
        super().__init__("telemetry", WIFI_USER_ADDR, WIFI_TELEM_PORT, 1024, 2.0)

    def start(self, _=None):
        super().start(on_data=self._handle_telemetry)

    def _handle_telemetry(self, data) -> None:
        telem_data = unpack_telemetry(data)
        bus.set(telem_data)
