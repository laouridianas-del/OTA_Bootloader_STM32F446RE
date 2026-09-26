import sys
import os

# Get the directory where this script is saved
script_dir = os.path.dirname(os.path.abspath(__file__))

bin_file = os.path.join(script_dir, "ota_image.bin")
c_file = os.path.join(script_dir, "ota_image.h")
array_name = "ota_image_bin"

try:
    with open(bin_file, "rb") as f:
        data = f.read()

    with open(c_file, "w") as f:
        f.write(f"const unsigned char {array_name}[] = {{\n")
        for i, b in enumerate(data):
            f.write(f"0x{b:02X}, ")
            if (i + 1) % 12 == 0:
                f.write("\n")
        f.write("\n};\n")
        f.write(f"const unsigned int {array_name}_len = {len(data)};\n")

    print(f"Success! Output saved to: {c_file}")

except FileNotFoundError:
    print(f"Error: Could not find '{bin_file}'")
    print(f"Please ensure 'ota_image.bin' is placed in: {script_dir}")