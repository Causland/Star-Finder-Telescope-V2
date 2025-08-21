from threading import Lock, Event
from typing import Tuple

class CameraDataBus:
    def __init__(self) -> None:
        self._lock = Lock()
        self._evt = Event()
        self._latest: str = ""

    def set(self, image: str) -> None:
        with self._lock:
            self._latest = image
            self._evt.set()

    def snapshot(self) -> str:
        with self._lock:
            return self._latest

    def to_dict(self) -> dict:
        image = self.snapshot()
        d = dict()
        d["image"] = image
        return d

    def wait_for_update(self, timeout: float | None = None) -> bool:
        ok = self._evt.wait(timeout)
        if ok:
            self._evt.clear()
        return ok

camera_databus = CameraDataBus()