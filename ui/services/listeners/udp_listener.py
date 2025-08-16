import socket
import threading
import time
from typing import Callable

class UDPListener:
    def __init__(self, name: str, ip: str, port: int, recv_buffer: int = 1024, retry_interval: float = 2.0):
        self.name = name
        self.ip = ip
        self.port = port
        self.recv_buffer = recv_buffer
        self.retry_interval = retry_interval

        self._socket = None
        self._thread = None
        self._stop_event = threading.Event()
        self._lock = threading.Lock()
        self._on_data = None

    def start(self, on_data: Callable = None) -> None:
        with self._lock:
            if self._thread and self._thread.is_alive():
                return
            self._on_data = on_data
            self._thread = threading.Thread(target=self._run, name=f"{self.name}", daemon=True)
            self._thread.start()

    def stop(self) -> None:
        self._stop_event.set()
        if self._socket:
            try:
                self._socket.close()
            except:
                pass

    def _run(self) -> None:
        while not self._stop_event.is_set():
            try:
                self._socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                self._socket.bind((self.ip, self.port))
                self._listen_loop()
            except OSError:
                time.sleep(self.retry_interval)
            finally:
                if self._socket:
                    self._socket.close()
                self._socket = None

    def _listen_loop(self) -> None:
        while not self._stop_event.is_set():
            try:
                data, _ = self._socket.recvfrom(self.recv_buffer)
                if self._on_data:
                    self._on_data(data)
            except:
                break