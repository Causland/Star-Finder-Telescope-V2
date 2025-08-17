from models.telemetry import unpack_telemetry
from services.databus.telemetry_databus import telem_databus
from services.listeners.udp_listener import UDPListener
from services.network_constants import WIFI_USER_ADDR, WIFI_TELEM_PORT

class TelemetryListener(UDPListener):
    def __init__(self):
        super().__init__("telemetry", WIFI_USER_ADDR, WIFI_TELEM_PORT, 1024, 2.0)

    def start(self) -> None:
        super().start(self._handle_telemetry)

    def _handle_telemetry(self, data) -> None:
        telem_data = unpack_telemetry(data)
        telem_databus.set(telem_data)
