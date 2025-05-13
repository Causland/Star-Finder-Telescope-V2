# Comment out all lines which define swap in Arducam header. This conflicts with
# std::swap calls in ESP32 compiler

import os

arducam_h_path = os.path.join(".pio", "libdeps", "esp_wroom_32", "ArduCAM", "ArduCAM.h")

lines = []
with open(arducam_h_path, "r") as file:
    for line in file:
      line = line.strip()
      if line.startswith("#define swap"):
        line = line.replace("#define", "//#define")

      lines.append(line)

with open(arducam_h_path, "w") as file:
    for line in lines:
        file.write(line)
        file.write("\n")