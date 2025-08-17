from dataclasses import asdict
from models.telemetry import TelemData
from threading import Lock, Event
from typing import Tuple

class TelemetryDataBus:
    def __init__(self) -> None:
        self._lock = Lock()
        self._evt = Event()
        self._latest: TelemData = TelemData()
        self._seq: int = 0 # Sequence number to avoid duplicate updates

    def set(self, telem: TelemData) -> None:
        with self._lock:
            self._latest = telem
            self._seq += 1
            self._evt.set()

    def snapshot(self) -> Tuple[TelemData, int]:
        with self._lock:
            return self._latest, self._seq

    def to_dict(self) -> dict:
        telem, seq = self.snapshot()
        d = asdict(telem)
        d["_seq"] = seq
        return d

    def wait_for_update(self, timeout: float | None = None) -> bool:
        ok = self._evt.wait(timeout)
        if ok:
            self._evt.clear()
        return ok

telem_databus = TelemetryDataBus()