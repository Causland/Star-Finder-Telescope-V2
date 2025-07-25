import socket
import struct

def format_version(val):
  maj = (val >> 16) & 0xFF
  min = (val >> 8) & 0xFF
  pat = (val) & 0xFF
  return f"v{maj}.{min}.{pat}"

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8001
udp_socket.bind((local_address, local_port))

while True:
  # Receive data from the socket
  data, addr = udp_socket.recvfrom(1024)
  
  format = "!IIIHfffBBBqdddff"
  unpacked_data = struct.unpack(format, data)
  vals = [
    format_version(val) if i == 0 else (f"{val:.3f}" if isinstance(val, float) else val)
    for i, val in enumerate(unpacked_data)
  ]

  print(f"Received data: {vals}")