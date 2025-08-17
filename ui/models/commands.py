from dataclasses import dataclass
from enum import Enum
import struct
from typing import List

class CommandID(Enum):
    TELEM_RATE = 0x00
    PLAN_TRAJECTORY = 0x01
    MOVE_BASE_SERVOS = 0x02
    CONTROL_CAMERA = 0x03
    FOCUS = 0x04
    OTA_UPDATE = 0x05

class ControlCameraCmdID(Enum):
    CONFIG = 0x00
    PHOTO = 0x01
    VIDEO = 0x02

class FocusCmdID(Enum):
    MANUAL = 0x00
    AUTO = 0x01

@dataclass
class TrajectoryEntry:
    rel_time: float
    target_az: float
    target_el: float

def pack_telem_rate(rate: int) -> bytes:
    return struct.pack("!BH", CommandID.TELEM_RATE.value, rate)

def pack_plan_trajectory(part_num: int, num_parts: int, entries: List[TrajectoryEntry]) -> bytes:
    format_str = "!BBBB" + "f" * len(entries)
    return struct.pack(format_str, CommandID.PLAN_TRAJECTORY.value,
                                   part_num,
                                   num_parts,
                                   len(entries))

def pack_move_base_servos(target_az: float, target_el: float) -> bytes:
    return struct.pack("!Bff", CommandID.MOVE_BASE_SERVOS.value, target_az, target_el)

def pack_control_camera_config(config_id: int, value: int) -> bytes:
    return struct.pack("!BBBB", CommandID.CONTROL_CAMERA.value,
                                ControlCameraCmdID.CONFIG.value,
                                config_id,
                                value)

def pack_control_camera_photo() -> bytes:
    return struct.pack("!BB", CommandID.CONTROL_CAMERA.value,
                              ControlCameraCmdID.PHOTO.value)

def pack_control_camera_video(duration: int) -> bytes:
    return struct.pack("!BBI", CommandID.CONTROL_CAMERA.value,
                               ControlCameraCmdID.VIDEO.value,
                               duration)

def pack_focus_manual(degrees: float) -> bytes:
    return struct.pack("!BBf", CommandID.FOCUS.value, FocusCmdID.MANUAL.value, degrees)

def pack_focus_auto() -> bytes:
    return struct.pack("!BB", CommandID.FOCUS.value, FocusCmdID.AUTO.value)

def pack_ota_update(filename: str) -> bytes:
    format_str = f"!BB{len(filename) + 1}s"
    return struct.pack(format_str, CommandID.OTA_UPDATE.value,
                                   len(filename) + 1,
                                   filename.encode('ascii'))
