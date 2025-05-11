# Enable the correct camera in the memorysaver.h file

import os

platform_name = "OV2640_MINI_2MP"

memsaver_path = os.path.join(".pio", "libdeps", "uno_r4_wifi", "ArduCAM", "memorysaver.h")

lines = []
with open(memsaver_path, "r") as file:
    for line in file:
      line = line.strip()
      if line.startswith("#define"):
         if not "_MEMORYSAVER_" in line and \
            not line.endswith(platform_name):
            line = line.replace("#define", "//#define")
      
      if line.endswith(platform_name):
         line = line.replace("//#define", "#define")

      lines.append(line)

with open(memsaver_path, "w") as file:
    for line in lines:
        file.write(line)
        file.write("\n")
      
