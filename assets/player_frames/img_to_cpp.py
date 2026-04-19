from PIL import Image
# import sys

DIM = 16
ORIGINAL_FILE = "up.png"

def generate_rotated(name, angle):
    img = Image.open(ORIGINAL_FILE).convert("RGBA").resize((DIM, DIM)).rotate(angle, resample=Image.BILINEAR)
    img.save(name)

def convert(path, name):
    img = Image.open(path).convert("RGBA").resize((DIM, DIM))
    pixels = []
    
    for r, g, b, a in img.getdata():
        if a < 128:
            pixels.append("Void")  # Void (transparent "magenta key")
        else:
            pixels.append("Black")
    
    rows = []
    for i in range(0, DIM*DIM, DIM):
        rows.append("    " + ", ".join(pixels[i:i+DIM]))
    
    # print(f"#pragma once")
    # print(f"#include <eadk.h>")
    print(f"")
    print(f"const EADK::Color {name}[{DIM*DIM}] = {{")
    print(",\n".join(rows))
    print(f"}};")

generate_rotated("down.png", -90)
generate_rotated("halfdown.png", -67.5)
generate_rotated("middle.png", -45)
generate_rotated("halfup.png", -22.5)


convert("down.png", "player_frame_down")
convert("halfdown.png", "player_frame_half_down")
convert("middle.png", "player_frame_middle")
convert("halfup.png", "player_frame_half_up")
convert(ORIGINAL_FILE, "player_frame_up")


# if len(sys.argv) < 3:
#     print("Usage: python3 img_to_cpp.py <image> <nom>")
#     sys.exit(1)

# convert(sys.argv[1], sys.argv[2])