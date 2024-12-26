# convert .ttf to .h to bake into .exe
with open("NeueHaasDisplay-Light.ttf", "rb") as f:
    font_data = f.read()

with open("NHG_light.h", "w") as header_file:
    header_file.write("const unsigned char font_data[] = {")
    for byte in font_data:
        header_file.write(f"0x{byte:02X}, ")
    header_file.write("};")
