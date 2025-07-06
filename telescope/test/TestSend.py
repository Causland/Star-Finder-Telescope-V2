import socket
import struct
import time

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

send_address = "192.168.4.1"
send_port = 8000

while True:
  # Send data
  data = "Hello World!\n"
  udp_socket.sendto(data.encode('utf-8'), (send_address, send_port))
  print(data)
  time.sleep(1)