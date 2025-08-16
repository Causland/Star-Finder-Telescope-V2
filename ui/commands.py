import struct
from enum import Enum

class CommandID(Enum):
    TELEM_RATE = 0x01
    PLAN_TRAJECTORY = 0x02
    MOVE_BASE_SERVOS = 0x03
    CONTROL_CAMERA = 0x04
    FOCUS = 0x05
    OTA_UPDATE = 0x06

def pack_telem_rate(rate: float) -> bytes:
    return struct.pack("!BH", CommandID.TELEM_RATE.value, rate)