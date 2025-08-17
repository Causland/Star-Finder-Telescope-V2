import asyncio
import json
from quart import Quart, websocket
from typing import Any

class WebSocket:
    def __init__(self, quart_app: Quart, ws_path: str, databus: Any):
        self._clients = set()
        self._ws_path = ws_path
        self._databus = databus
        self._register_route(quart_app)

    def _register_route(self, quart_app: Quart) -> None:
        endpoint_name = f"{type(self).__name__}_{self._ws_path}"

        @quart_app.websocket(self._ws_path, endpoint=endpoint_name)
        async def log_stream():
            # Register this client
            self._clients.add(websocket._get_current_object())
            try:
                # Keep the connection open
                while True:
                    await asyncio.sleep(30)  # passive keepalive
            finally:
                self._clients.discard(websocket._get_current_object())

    async def broadcast_updates(self) -> None:
        while True:
            await asyncio.to_thread(self._databus.wait_for_update)  # run blocking wait in thread
            payload = json.dumps(self._databus.to_dict())
            stale = set()
            for ws in list(self._clients):
                try:
                    await ws.send(payload)
                except Exception:
                    stale.add(ws)
            for ws in stale:
                self._clients.discard(ws)
