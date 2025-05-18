import socket
import struct

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.1.3"
local_port = 8001
udp_socket.bind((local_address, local_port))

while True:
  # Receive data from the socket
  data, addr = udp_socket.recvfrom(1024)
  
  print(data)