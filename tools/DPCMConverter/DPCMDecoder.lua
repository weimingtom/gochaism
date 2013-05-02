-- DPCM Decoder (DMC to 8bit PCM)
-- Note: spwave can import raw 8bit wave data

if #arg < 3 then
	print("DPCMDecoder.lua <input file> <output file> <dmc initial volume>")
	return
end

local infilename = arg[1]
local outfilename = arg[2]
local infile = io.open(infilename, "rb")
local outfile = io.open(outfilename, "wb")

local dmc_bytes = {}
local dmc_byte = infile:read(1)
while dmc_byte do
	dmc_byte = dmc_byte:byte()
	table.insert(dmc_bytes, dmc_byte)
	dmc_byte = infile:read(1)
end

local dmc_value = tonumber(arg[3])
local dmc_out8bit = true
local dmc_loop = 0
for i = 0, dmc_loop do
	for dmc_index, dmc_byte in ipairs(dmc_bytes) do
		for dmc_biti = 0, 7 do
			local dmc_bit = math.floor(dmc_byte / 2^dmc_biti) % 2
			if dmc_bit ~= 0 then
				if dmc_value < 126 then
					dmc_value = dmc_value + 2
				end
			else
				if dmc_value > 1 then
					dmc_value = dmc_value - 2
				end
			end

			local v = dmc_value
			if dmc_out8bit then
				v = dmc_value * 2
				--v = (v >= 128) and (v - 128) or (v + 128) -- signed 8bit
			end
			if dmc_loop == 0 or i == 1 then
				outfile:write(string.char(v))
			end
		end
	end
end

infile:close()
outfile:close()
