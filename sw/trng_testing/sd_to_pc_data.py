from labjack import ljm

# Open T7-Pro
handle = ljm.openS("T7", "USB", "ANY")

# Read binary file from SD card
filename = "/trng_data.bin"
with open("outputs/trng_data_local.bin", "wb") as f_local:
    offset = 0
    # 4kb at a time
    block_size = 4096 
    while True:
        data = ljm.eReadNameByteArray(handle, filename, block_size)
        if not data:
            break
        f_local.write(bytes(data))
        offset += len(data)

ljm.close(handle)
