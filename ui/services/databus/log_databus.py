from threading import Lock, Event
from typing import Tuple

class LogDataBus:
    def __init__(self) -> None:
        self._lock = Lock()
        self._evt = Event()
        self._latest: str = ""
        self._seq: int = 0 # Sequence number to avoid duplicate updates

    def set(self, log: str) -> None:
        with self._lock:
            self._latest = log
            self._seq += 1
            self._evt.set()

    def snapshot(self) -> Tuple[str, int]:
        with self._lock:
            return self._latest, self._seq

    def to_dict(self) -> dict:
        log, seq = self.snapshot()
        d = dict()
        d["log_data"] = log
        d["_seq"] = seq
        return d

    def wait_for_update(self, timeout: float | None = None) -> bool:
        ok = self._evt.wait(timeout)
        if ok:
            self._evt.clear()
        return ok

log_databus = LogDataBus()