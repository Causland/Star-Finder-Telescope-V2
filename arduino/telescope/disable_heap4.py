# Renaming heap_4.c to disable it for compilation. PlatformIO does not
# provide a way to filter source files from libraries, so we have to rename it.

import os

# Get user's home directory
home_dir = os.path.expanduser("~")

# Define the relative path to heap_4.c within PlatformIO
heap4_path = os.path.join(
    home_dir,
    ".platformio/packages/framework-arduinorenesas-uno/libraries/Arduino_FreeRTOS/src/portable/MemMang/heap_4.c"
)

# Check if file exists and rename to disable it
if os.path.exists(heap4_path):
    os.rename(heap4_path, heap4_path + ".disabled")
    print("heap_4.c has been disabled for compilation.")
