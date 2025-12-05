--[[
    Name: lj_data.lua
    Desc: This script records GPIO pin state as a 1 or 0 and writes to a binary file
    Note: Requires an SD Card installed inside the T7 or T7-Pro
          This example requires firmware 1.0282 (T7)
--]]

-- Read information about the hardware installed
local hardware = MB.readName("HARDWARE_INSTALLED")
local passed = 1
-- If the seventh bit is not a 1 then an SD card is not installed
if(bit.band(hardware, 8) ~= 8) then
  print("uSD card not detected")
  passed = 0
end
if(passed == 0) then
  print("This Lua script requires a microSD card, but is not detected. These features are only preinstalled on the T7-Pro. Script Stopping")
  -- Writing a 0 to LUA_RUN stops the script
  MB.W("LUA_RUN", 0)
end

local filename = "/trng_data.bin"
local voltage = 0
local status = 0
local max_bits = 12500
local byte_acc = 0
local bit_count = 0
local total_bits = 0

-- Create and open a file for write access
local file = io.open(filename, "wb")
-- Make sure that the file was opened properly.
if file then
  print("Opened File on uSD Card")
else
  -- If the file was not opened properly we probably have a bad SD card.
  print("!! Failed to open file on uSD Card !!")
end

-- Read rate of 100kHz
LJ.IntervalConfig(0, 0.01)

while true do
    -- If an interval is done
    if LJ.CheckInterval(0) then
        -- Read TRNG bit
        status = MB.readName("FIO0")

        -- Load TRNG bit into a byte
        byte_acc = byte_acc + status * 2^(7 - bit_count)
        bit_count = bit_count + 1
        total_bits = total_bits + 1

        -- Write 8 bits to file
        if bit_count == 8 then
            file:write(string.char(byte_acc))
            byte_acc = 0
            bit_count = 0
        end
    end

    -- Builds a 100Mb file
    if total_bits >= max_bits then
        break
    end
end

file:close()
print("Done acquiring data! Now run python script to retrieve data. \n")
MB.W(6000, 1, 0)
