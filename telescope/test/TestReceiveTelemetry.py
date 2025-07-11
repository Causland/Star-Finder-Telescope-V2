import socket
import struct

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8001
udp_socket.bind((local_address, local_port))

while True:
  # Receive data from the socket
  data, addr = udp_socket.recvfrom(1024)
  
  format = "!IIHfffBBBIdddff"
  unpacked_data = struct.unpack(format, data)
  vals = [f"{val:.3f}" if isinstance(val, float) else val for val in unpacked_data]

  print(f"Received data: {vals}")