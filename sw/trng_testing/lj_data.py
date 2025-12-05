from labjack import ljm
from time import sleep

status = 0
byte_acc = 0
bit_count = 0
total_bits = 0
name = "DIO0"

# TODO: Update if needed: 100 bitstreams for the STS (1M per stream)
max_bits = 100_000_000
# TODO: Update filepath if needed
output_dir = "../Projects/sts-2.1.2/data/outputs" 
f_seq = open(f"trng_results.data", "wb")

# Open T7-Pro
handle = ljm.openS("T7", "USB", "ANY")

# Disable any extended features
ljm.eWriteName(handle, "DIO0_EF_ENABLE", 0)  

# Create and open a file for write access
while True:
    # Builds 100 files
    if total_bits >= max_bits:
        break

    # Read TRNG bit
    try:
        status = int(ljm.eReadName(handle, name))

        # Load TRNG bit into a byte
        byte_acc += status * 2**(7 - bit_count)
        bit_count += 1
        total_bits += 1

        # Write 8 bits to file
        if bit_count == 8:
            f_seq.write(bytes([byte_acc]))
            byte_acc = 0
            bit_count = 0
            print(f"Writing bit {total_bits}")
        sleep(0.0003)
    except ljm.LJMError as e:
        #print(f"Read failed, skipping: {e}")
        continue

print("Done acquiring data! \n")

# Flush remaining bits
if bit_count > 0:
    f_seq.write(bytes([byte_acc]))  
f_seq.close()

ljm.close(handle)
