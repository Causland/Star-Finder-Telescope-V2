import base64
from services.databus.camera_databus import camera_databus
from services.listeners.udp_listener import UDPListener
from services.network_constants import WIFI_USER_ADDR, WIFI_CAMERA_PORT

JPEG_SOI = b"\xFF\xD8"
JPEG_EOI = b"\xFF\xD9"

class ImageAssembler:
    def __init__(self):
        self._buf = bytearray()

    def append_chunk(self, chunk: bytes) -> str | None:
        # JPEG files start with FFD8 and end with FFD9
        if len(self._buf) == 0:
            soi_index = chunk.find(JPEG_SOI)
            if soi_index != -1:
                chunk = chunk[soi_index:]
            else:
                return None # Invalid start to processing
        else:
            eoi_index = chunk.find(JPEG_EOI)
            if eoi_index != -1:
                chunk = chunk[:eoi_index + len(JPEG_EOI)]

        self._buf.extend(chunk)

        if self._buf.endswith(JPEG_EOI):
            encoded = base64.b64encode(self._buf).decode("utf-8")
            self._buf.clear()
            return encoded
        return None

class CameraListener(UDPListener):
    def __init__(self):
        super().__init__("camera", WIFI_USER_ADDR, WIFI_CAMERA_PORT, 1024, 2.0),
        self._image_assembler = ImageAssembler()

    def start(self) -> None:
        super().start(self._handle_camera_data)

    def _handle_camera_data(self, data) -> None:
        image = self._image_assembler.append_chunk(data)
        
        if image is not None:
            camera_databus.set(f"data:image/jpeg;base64,{image}")
