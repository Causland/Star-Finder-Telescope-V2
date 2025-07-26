import socket
import struct
import sys

if len(sys.argv) > 1:  # Ensure an argument was provided
    filename = sys.argv[1]
else:
    print("Usage: python TestOTAUpdate.py <filename>")

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8000
udp_socket.bind((local_address, local_port))

target_address = "192.168.4.1"
target_port = 8000

# Send an OTA update command
print("Sending update command")
format_str = f"!BB{len(filename) + 1}s"
packet = struct.pack(format_str, 5, len(filename) + 1, filename.encode('ascii'))
udp_socket.sendto(packet, (target_address, target_port))