import socket
import struct
import time
from functools import reduce

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8000
udp_socket.bind((local_address, local_port))

target_address = "192.168.4.1"
target_port = 8000

# Trajectory packet in the format:
# <cmd><seqNum><totalParts><numEntries><checksum><entries>

# Send a trajectory with one part and max entries
print("Sending trajectory with one part and max entries")
vals = [0.0, 0.0, 30.0,
        5.0, 30.0, 40.0,
        10.0, 60.0, 60.0,
        15.0, 180.0, 45.0,
        20.0, 210.0, 80.0,
        25.0, 120.0, 60.0,
        30.0, 80.0, 50.0,
        35.0, 40.0, 40.0,
        40.0, 30.0, 30.0,
        45.0, 20.0, 45.0]
format_str = "!BBBB" + "f" * len(vals)
packet = struct.pack(format_str, 1, 1, 1, 10, *vals)
udp_socket.sendto(packet, (target_address, target_port))