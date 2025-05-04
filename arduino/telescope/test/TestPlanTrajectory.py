import socket
import struct
import time

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
local_address = "192.168.1.3"
local_port = 8000
udp_socket.bind((local_address, local_port))

target_address = "192.168.1.2"
target_port = 8000

# Trajectory packet in the format:
# <cmd><seqNum><totalParts><numEntries><checksum><entries>

# Send a trajectory with one part and one entry
print("Sending trajectory with one part and one entry")
packet = struct.pack("<BBBBBfff", 1, 1, 1, 1, 224, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)

# Send a trajectory with one part and two entries
print("Sending trajectory with one part and two entries")
packet = struct.pack("<BBBBBffffff", 1, 1, 1, 2, 158, 1.0, 2.2, 3.3, 
                                                    2.0, 4.4, 5.5)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(3)

# Send a trajectory with one part and max entries
print("Sending trajectory with one part and max entries")
vals = [1.0, 2.2, 3.3,
        2.0, 4.4, 5.5,
        3.0, 6.6, 7.7,
        4.0, 8.8, 9.9,
        5.0, 10.10, 11.11,
        6.0, 12.12, 13.13,
        7.0, 14.14, 15.15,
        8.0, 16.16, 17.17,
        9.0, 18.18, 19.19,
        10.0, 20.20, 21.21]
format_str = "<BBBBB" + "f" * len(vals)
packet = struct.pack(format_str, 1, 1, 1, 10, 162, *vals)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(11)

# Send a trajectory with two parts, each with one entry
print("Sending trajectory with two parts, each with one entry")
packet = struct.pack("<BBBBBfff", 1, 1, 2, 1, 227, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 2, 2, 1, 124, 2.0, 4.4, 5.5)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(3)

# Send a trajectory with two parts, each with two entries
print("Sending trajectory with two parts, each with two entries")
packet = struct.pack("<BBBBBffffff", 1, 1, 2, 2, 157, 1.0, 2.2, 3.3,
                                                    2.0, 4.4, 5.5)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBffffff", 1, 2, 2, 2, 244, 3.0, 6.6, 7.7,
                                                    4.0, 8.8, 9.9)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(5)

# Send a trajectory with two parts, each with max entries
print("Sending trajectory with two parts, each with max entries")
vals = [1.0, 2.2, 3.3,
        2.0, 4.4, 5.5,
        3.0, 6.6, 7.7,
        4.0, 8.8, 9.9,
        5.0, 10.10, 11.11,
        6.0, 12.12, 13.13,
        7.0, 14.14, 15.15,
        8.0, 16.16, 17.17,
        9.0, 18.18, 19.19,
        10.0, 20.20, 21.21]
format_str = "<BBBBB" + "f" * len(vals)
packet = struct.pack(format_str, 1, 1, 2, 10, 161, *vals)
udp_socket.sendto(packet, (target_address, target_port))
vals = [11.0, 22.22, 23.23,
        12.0, 24.24, 25.25,
        13.0, 26.26, 27.27,
        14.0, 28.28, 29.29,
        15.0, 30.30, 31.31,
        16.0, 32.32, 33.33,
        17.0, 34.34, 35.35,
        18.0, 36.36, 37.37,
        19.0, 38.38, 39.39,
        20.0, 40.40, 41.41]
packet = struct.pack(format_str, 1, 2, 2, 10, 224, *vals)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(21)

# Send a trajectory with four parts, each with one entry
print("Sending trajectory with four parts, each with one entry")
packet = struct.pack("<BBBBBfff", 1, 1, 4, 1, 229, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 2, 4, 1, 122, 2.0, 4.4, 5.5)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 3, 4, 1, 35, 3.0, 6.6, 7.7)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 4, 4, 1, 210, 4.0, 8.8, 9.9)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(5)

# Send a trajectory with four parts, each with max entries
print("Sending trajectory with four parts, each with max entries")
vals = [1.0, 2.2, 3.3,
        2.0, 4.4, 5.5,
        3.0, 6.6, 7.7,
        4.0, 8.8, 9.9,
        5.0, 10.10, 11.11,
        6.0, 12.12, 13.13,
        7.0, 14.14, 15.15,
        8.0, 16.16, 17.17,
        9.0, 18.18, 19.19,
        10.0, 20.20, 21.21]
format_str = "<BBBBB" + "f" * len(vals)
packet = struct.pack(format_str, 1, 1, 4, 10, 167, *vals)
udp_socket.sendto(packet, (target_address, target_port))
vals = [11.0, 22.22, 23.23,
        12.0, 24.24, 25.25,
        13.0, 26.26, 27.27,
        14.0, 28.28, 29.29,
        15.0, 30.30, 31.31,
        16.0, 32.32, 33.33,
        17.0, 34.34, 35.35,
        18.0, 36.36, 37.37,
        19.0, 38.38, 39.39,
        20.0, 40.40, 41.41]
packet = struct.pack(format_str, 1, 2, 4, 10, 230, *vals)
udp_socket.sendto(packet, (target_address, target_port))
vals = [21.0, 42.42, 43.43,
        22.0, 44.44, 45.45,
        23.0, 46.46, 47.47,
        24.0, 48.48, 49.49,
        25.0, 50.50, 51.51,
        26.0, 52.52, 53.53,
        27.0, 54.54, 55.55,
        28.0, 56.56, 57.57,
        29.0, 58.58, 59.59,
        30.0, 60.60, 61.61]
packet = struct.pack(format_str, 1, 3, 4, 10, 252, *vals)
udp_socket.sendto(packet, (target_address, target_port))
vals = [31.0, 62.62, 63.63,
        32.0, 64.64, 65.65,
        33.0, 66.66, 67.67,
        34.0, 68.68, 69.69,
        35.0, 70.70, 71.71,
        36.0, 72.72, 73.73,
        37.0, 74.74, 75.75,
        38.0, 76.76, 77.77,
        39.0, 78.78, 79.79,
        40.0, 80.80, 81.81]
packet = struct.pack(format_str, 1, 4, 4, 10, 161, *vals)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(41)

# Send a trajectory with out of order parts
print("Sending trajectory with out of order parts")
packet = struct.pack("<BBBBBfff", 1, 2, 4, 1, 230, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 1, 4, 1, 121, 2.0, 4.4, 5.5)
udp_socket.sendto(packet, (target_address, target_port))
packet = struct.pack("<BBBBBfff", 1, 4, 4, 1, 36, 3.0, 6.6, 7.7)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)

# Send a trajectory with too many parts
print("Sending trajectory with too many parts")
packet = struct.pack("<BBBBBfff", 1, 1, 5, 1, 228, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)

# Send a trajectory with an invalid checksum
print("Sending trajectory with an invalid checksum")
packet = struct.pack("<BBBBBfff", 1, 1, 1, 1, 0, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)

# Send a trajectory with an invalid number of entries
print("Sending trajectory with an invalid number of entries")
packet = struct.pack("<BBBBBfff", 1, 1, 1, 11, 191, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)

# Send a trajectory with missing entries
print("Sending trajectory with missing entries")
packet = struct.pack("<BBBBBfff", 1, 1, 1, 2, 203, 1.0, 2.2, 3.3)
udp_socket.sendto(packet, (target_address, target_port))
time.sleep(2)