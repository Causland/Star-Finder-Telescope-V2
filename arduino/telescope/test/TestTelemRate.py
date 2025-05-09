import socket
import struct
import sys
import time
from functools import reduce

if len(sys.argv) > 1:  # Ensure an argument was provided
    try:
        rate = int(sys.argv[1])  # Convert the first argument to a int
    except ValueError:
        print("Error: Please provide a valid rate in ms.")
else:
    print("Usage: python TestTelemRate.py <rate>")

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.1.3"
local_port = 8000
udp_socket.bind((local_address, local_port))

target_address = "192.168.1.2"
target_port = 8000

# Send a rate update
print("Sending telemetry rate update")
format_str = "<BH"
packet = struct.pack(format_str, 0, rate)
udp_socket.sendto(packet, (target_address, target_port))