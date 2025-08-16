from dataclasses import dataclass
import struct

@dataclass
class TelemData:
    version : str = "v0.0.0"
    time : float = 0.0
    free_heap : int = 0
    cmds_received : int = 0
    gps_lat : float = 0.0
    gps_lon : float = 0.0
    gps_alt : float = 0.0
    traj_running : bool = False
    traj_num_entries : int = 0
    traj_current_entry : int = 0
    time_to_next : float = 0.0
    curr_az : float = 0.0
    curr_el : float = 0.0
    speed_az : float = 0.0
    target_az : float = 0.0
    target_el : float = 0.0

    @staticmethod
    def format_version(val: int) -> str:
        maj = (val >> 16) & 0xFF
        min = (val >> 8) & 0xFF
        pat = (val) & 0xFF
        return f"v{maj}.{min}.{pat}"

def unpack_telemetry(data: bytes) -> TelemData:
    format = "!IIIHfffBBBqdddff"
    unpacked_data = struct.unpack(format, data)

    # Assign fields explicitly
    (
        version_int, time, free_heap, cmds_received,
        gps_lat, gps_lon, gps_alt,
        traj_running, traj_num_entries, traj_current_entry, time_to_next,
        curr_az, curr_el, speed_az, target_az, target_el
    ) = unpacked_data

    return TelemData(
        version = TelemData.format_version(version_int),
        time = time / 1000000.0,
        free_heap = free_heap,
        cmds_received = cmds_received,
        gps_lat = gps_lat,
        gps_lon = gps_lon,
        gps_alt = gps_alt,
        traj_running = bool(traj_running),
        traj_num_entries = traj_num_entries,
        traj_current_entry = traj_current_entry,
        time_to_next = time_to_next,
        curr_az = curr_az,
        curr_el = curr_el,
        speed_az = speed_az,
        target_az = target_az,
        target_el = target_el
    )
