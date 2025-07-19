import socket
import struct

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.4.2"
local_port = 8002
udp_socket.bind((local_address, local_port))

target_address = "192.168.4.1"
target_port = 8000

# Send a take photo command
print("Sending take photo command")
format_str = "!BB"
packet = struct.pack(format_str, 3, 1)
udp_socket.sendto(packet, (target_address, target_port))


# Receive data from the socket
data, addr = udp_socket.recvfrom(1024)

with open("test.jpg", "wb") as f:
    while len(data) >= 508:
        f.write(data)
        data, addr = udp_socket.recvfrom(1024)
    f.write(data)