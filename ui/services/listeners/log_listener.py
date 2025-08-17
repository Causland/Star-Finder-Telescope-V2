from services.databus.log_databus import log_databus
from services.listeners.udp_listener import UDPListener
from services.network_constants import WIFI_USER_ADDR, WIFI_LOG_PORT

class LogListener(UDPListener):
    def __init__(self):
        super().__init__("log", WIFI_USER_ADDR, WIFI_LOG_PORT, 1024, 2.0)

    def start(self) -> None:
        super().start(self._handle_log_data)

    def _handle_log_data(self, data) -> None:
        log_data = data.decode('ascii', errors='replace')
        log_databus.set(log_data)
