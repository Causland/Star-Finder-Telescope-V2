import socket
import struct
import sys

if len(sys.argv) > 1:  # Ensure an argument was provided
    try:
        degrees = float(sys.argv[1])  # Convert the first argument to a int
    except ValueError:
        print("Error: Please provide a valid offset in deg.")
else:
    print("Usage: python TestFocus.py <degrees>")

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8000
udp_socket.bind((local_address, local_port))

target_address = "192.168.4.1"
target_port = 8000

# Send a focus command
print("Sending focus command")
format_str = "!BBf"
packet = struct.pack(format_str, 4, 0, degrees)
udp_socket.sendto(packet, (target_address, target_port))