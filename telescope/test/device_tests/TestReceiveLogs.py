import socket
import struct

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8004
udp_socket.bind((local_address, local_port))

while True:
  # Receive data from the socket
  data, addr = udp_socket.recvfrom(512)
  try:
    print(data.decode('ascii', errors='replace'), end='')
  except Exception as e:
    print(f"Decode error: {e}")