-- FlMML PCM encoder
-- written by http://twitter.com/gochaism

local FlMMLTimeBase = 384
local VolMacroSymbols = "ABCDEFGHIJKLMNOPQRSTUVabcdefghijklmnopqrstuv"
local LenMacroSymbols = "WXYZwxyz"

function round(d)
	return math.floor(d + 0.5)
end

function clip(minValue, d, maxValue)
	return math.max(minValue, math.min(d, maxValue))
end

function ReadWaveFile(path)
	local read1 = function(file)
		return string.byte(file:read(1))
	end
	local read2 = function(file)
		local l = read1(file)
		local h = read1(file)
		return h * 0x100 + l
	end
	local read2s = function(file)
		local value = read2(file)
		if value < 0x8000 then
			return value
		else
			return value - 0x10000
		end
	end
	local read3 = function(file)
		local l = read1(file)
		local m = read1(file)
		local h = read1(file)
		return h * 0x10000 + m * 0x100 + l
	end
	local read3s = function(file)
		local value = read3(file)
		if value < 0x800000 then
			return value
		else
			return value - 0x1000000
		end
	end
	local read4 = function(file)
		local l = read2(file)
		local h = read2(file)
		return h * 0x10000 + l
	end

	local wave = {}
	local info = {}

	local file = io.open(path, "rb")
	if file == nil then
		return wave, info
	end

	-- verify RIFF/WAVE header
	if file:read(4) ~= "RIFF" then
		return wave, info
	end
	file:seek("cur", 4)
	if file:read(8) ~= "WAVEfmt " then
		return wave, info
	end
	local fmtChunkSize = read4(file)

	-- parse format header
	info.format = read2(file)
	info.channels = read2(file)
	info.samplerate = read4(file)
	local bytesPerSeconds = read4(file)
	local bytesPerBlock = read2(file) -- bytes/sample * channels
	info.bitwidth = read2(file)
	file:seek("cur", fmtChunkSize - 0x10)

	-- check unsupported format
	if info.format ~= 1 or (info.bitwidth ~= 8 and info.bitwidth ~= 16 and info.bitwidth ~= 24) then
		return wave, info
	end

	-- data chunk
	if file:read(4) ~= "data" then
		return wave, info
	end
	local dataChunkSize = read4(file)
	assert(dataChunkSize % bytesPerBlock == 0,
		"Invalid data chunk size (" .. dataChunkSize .. "/" .. bytesPerBlock .. ")")

	-- read whole samples to array
	for sampleIndex = 1, dataChunkSize / bytesPerBlock do
		for channelIndex = 1, info.channels do
			local value
			if info.bitwidth == 8 then
				value = (read1(file) - 0x80) / 0x80
			elseif info.bitwidth == 16 then
				value = read2s(file) / 0x8000
			elseif info.bitwidth == 24 then
				value = read3s(file) / 0x800000
			else
				error("Unsupported bitwidth " .. info.bitwidth)
			end

			if wave[channelIndex] == nil then
				wave[channelIndex] = {}
			end
			wave[channelIndex][sampleIndex] = value
		end
	end

	return wave, info
end

-- [ volume converter ] ---------------

assert(#VolMacroSymbols % 2 == 0)
local VolMacroPatternCount = #VolMacroSymbols / 2

function MakeVolCommandAbs(targetVolume)
	return "@v" .. targetVolume
end

function MakeVolCommandRel(curVolume, targetVolume)
	local diff = targetVolume - curVolume
	local distance = math.abs(diff)
	local volDiffInc, volDiffDec = "(", ")"
	if diff < 0 then
		volDiffInc, volDiffDec = volDiffDec, volDiffInc
	end

	local symbolIndex = distance - 2
	if distance > (VolMacroPatternCount + 2) then
		symbolIndex = math.floor(distance / 2) - 2
	end
	if diff < 0 then
		symbolIndex = symbolIndex + VolMacroPatternCount
	end
	local macroSymbol = ""
	if symbolIndex >= 1 and symbolIndex <= #VolMacroSymbols then
		macroSymbol = VolMacroSymbols:sub(symbolIndex, symbolIndex)
	end

	if distance == 0 then
		return ""
	elseif distance == 1 then
		return volDiffInc
	elseif distance == 2 then
		return volDiffInc .. volDiffInc
	else
		if distance <= (VolMacroPatternCount + 2) then
			assert(macroSymbol ~= "")
			return "$" .. macroSymbol
		elseif distance % 2 == 0 and distance >= 6 and distance <= 2 * (VolMacroPatternCount + 2) then
			assert(macroSymbol ~= "")
			return "$" .. macroSymbol .. "$" .. macroSymbol
		else
			return MakeVolCommandAbs(targetVolume)
		end
	end
end

function MakeVolCommand(curVolume, targetVolume)
	local volRel = MakeVolCommandRel(curVolume, targetVolume)
	local volAbs = MakeVolCommandAbs(targetVolume)
	if #volRel <= #volAbs then
		return volRel
	else
		return volAbs
	end
end

local VolMacroDefs = {}
if VolMacroPatternCount > 0 then
	local volIncMacros, volDecMacros = "", ""
	for index = 1, VolMacroPatternCount do
		local volIncMacroSymbol = VolMacroSymbols:sub(index, index)
		local volDecMacroSymbol = VolMacroSymbols:sub(VolMacroPatternCount + index, VolMacroPatternCount + index)

		local volIncMacro = "(("
		local volDecMacro = "))"
		for i = 1, index do
			volIncMacro = volIncMacro .. "("
			volDecMacro = volDecMacro .. ")"
		end
		VolMacroDefs[index * 2 - 1] = { name = volIncMacroSymbol, value = volIncMacro }
		VolMacroDefs[index * 2] = { name = volDecMacroSymbol, value = volDecMacro }
	end
end

-- [ length converter ] ---------------

local SamplesPerNote

local LengthToNoteTable = {}
local LenMacroDefs = {}
function ResetLengthToNoteTable()
	LengthToNoteTable = {}
	-- reset
	LengthToNoteTable[1] = "c"
	for sampLen = 2, SamplesPerNote * 2 do
		LengthToNoteTable[sampLen] = ""
	end
	-- single note + dots
	for sampLen = 2, SamplesPerNote do
		if SamplesPerNote % sampLen == 0 then
			local baseLen = SamplesPerNote / sampLen
			LengthToNoteTable[sampLen] = "c" .. baseLen

			local len = sampLen
			local dots = ""
			local dotLen = sampLen
			while dotLen % 2 == 0 do
				dotLen = dotLen / 2
				dots = dots .. "."

				len = len + dotLen
				if len <= #LengthToNoteTable then
					assert(LengthToNoteTable[len] == "")
					LengthToNoteTable[len] = "c" .. baseLen .. dots
				end
			end
		end
	end
	-- tick
	for sampLen = 2, #LengthToNoteTable do
		local tick = sampLen * (FlMMLTimeBase / SamplesPerNote)
		local tickstring = "c" .. "%" .. tostring(tick)
		if LengthToNoteTable[sampLen] == "" or #LengthToNoteTable[sampLen] > #tickstring then
			LengthToNoteTable[sampLen] = tickstring
		end
	end
	-- continuous notes
	for sampLen = 2, #LengthToNoteTable do
		if LengthToNoteTable[sampLen] == "" or #LengthToNoteTable[sampLen] > sampLen then
			local note = ""
			for index = 1, sampLen do
				note = note .. "c"
			end
			LengthToNoteTable[sampLen] = note
		end
	end
	-- long note
	for sampLen = SamplesPerNote + 1, SamplesPerNote * 2 do
		local note = "c" .. LengthToNoteTable[sampLen - SamplesPerNote]
		if #note <= #LengthToNoteTable[sampLen] then
			LengthToNoteTable[sampLen] = note
		end
	end
	-- combination
	for sampLen = 3, #LengthToNoteTable do
		if #LengthToNoteTable[sampLen] > 2 then
			for i = 1, math.floor(sampLen / 2) do
				j = sampLen - i
				local note = LengthToNoteTable[j] .. LengthToNoteTable[i]
				if #note < #LengthToNoteTable[sampLen] then
					LengthToNoteTable[sampLen] = note
				end
			end
		end
	end
	-- macros
	LenMacroDefs = {}
	if #LenMacroSymbols > 0 then
		for index = 1, #LenMacroSymbols do
			local sampLen = index + 2
			local macroSymbol =LenMacroSymbols:sub(index, index)
			LenMacroDefs[index] = { name = macroSymbol, value = LengthToNoteTable[sampLen] }
			LengthToNoteTable[sampLen] = "$" .. macroSymbol
		end
	end
	-- combination
	for sampLen = 3, #LengthToNoteTable do
		if #LengthToNoteTable[sampLen] > 2 then
			for i = 1, math.floor(sampLen / 2) do
				j = sampLen - i
				local note = LengthToNoteTable[j] .. LengthToNoteTable[i]
				if #note < #LengthToNoteTable[sampLen] then
					LengthToNoteTable[sampLen] = note
				end
			end
		end
	end
end

function MakeLenCommand(sampLen)
	local tick = sampLen * (FlMMLTimeBase / SamplesPerNote)
	local noteAbs = "c" .. "%" .. tostring(tick)

	local len = sampLen
	local noteRel = ""
	while len > SamplesPerNote * 2 do
		len = len - (SamplesPerNote * 2)
		noteRel = noteRel .. "c1c1"
	end
	noteRel = noteRel .. LengthToNoteTable[len]

	if #noteRel <= #noteAbs then
		return noteRel
	else
		return noteAbs
	end
end

-- [ core subroutines ] ---------------

local ShowHistogram = false
local LayeredPlayback = true
local Verbose = false

function SevenBitiseReal(d)
	return clip(0, (d + 1.0) * 0x40, 0x80)
end

function SevenBitise(d)
	return clip(0, math.floor((d + 1.0) * 0x40), 0x7f)
end

function PreprocessPCM(PCM)
	local PCM7 = {}
	-- local curVolume = SevenBitiseReal(PCM[1])
	for index = 1, #PCM do
		PCM7[index] = SevenBitise(PCM[index])
	-- 	local nextVolume = SevenBitiseReal(PCM[index])
	-- 	local diff = nextVolume - curVolume
	-- 	local distance = math.abs(diff)
	-- 	PCM7[index] = clip(0, round(curVolume + diff), 0x7f)
	-- 	curVolume = nextVolume
	end
	return PCM7
end

function MakeHistogram(PCM7)
	local VolHistogram = {}
	local LenHistogram = {}
	for index = -127, 127 do
		VolHistogram[index] = 0
	end
	for index = 1, SamplesPerNote * 2 + 1 do
		LenHistogram[index] = 0
	end

	if #PCM7 <= 1 then
		return VolHistogram, LenHistogram
	end

	local curVolume = PCM7[1]
	local sampLen = 0
	for index = 1, #PCM7 do
		local nextVolume = PCM7[index]
		local diff = nextVolume - curVolume
		local distance = math.abs(diff)
		if distance ~= 0 then
			if sampLen > SamplesPerNote then
				LenHistogram[SamplesPerNote * 2 + 1] = LenHistogram[SamplesPerNote * 2 + 1] + 1
			else
				LenHistogram[sampLen] = LenHistogram[sampLen] + 1
			end
			VolHistogram[diff] = VolHistogram[diff] + 1
			sampLen = 0
		end
		sampLen = sampLen + 1
		curVolume = nextVolume
	end
	if sampLen > SamplesPerNote then
		LenHistogram[SamplesPerNote * 2 + 1] = LenHistogram[SamplesPerNote * 2 + 1] + 1
	else
		LenHistogram[sampLen] = LenHistogram[sampLen] + 1
	end
	return VolHistogram, LenHistogram
end

function ToFlPCM(MMLFile, PCM, PCMInfo)
	if LayeredPlayback then
		SamplesPerNote = 96
	else
		SamplesPerNote = 96 -- 384
	end
	assert(FlMMLTimeBase % SamplesPerNote == 0)
	ResetLengthToNoteTable()

	local PCM7 = PreprocessPCM(PCM)
	local VolHistogram, LenHistogram = MakeHistogram(PCM7)

	if ShowHistogram then
		print("/*")
		print("#", "vol", "", "len")
		for index = -127, 193 do
			print(index,
				((index <= 127) and VolHistogram[index] or ""),
				((index <= 127) and (index >= 0 and MakeVolCommand(0, index) or MakeVolCommand(127, 127 + index)) or ""),
				((index >= 1 and index <= SamplesPerNote * 2 + 1) and LenHistogram[index] or ""),
				((index >= 1 and index <= SamplesPerNote * 2) and MakeLenCommand(index) or ""))
		end
		print("*/")
	end

	for index = 1, #VolMacroDefs do
		local diff = math.floor((index - 1) / 2) + 3
		if index % 2 == 0 then
			diff = -diff
		end
		if VolHistogram[diff] ~= 0 then
			MMLFile:write("$" .. VolMacroDefs[index].name .. "=" .. VolMacroDefs[index].value .. ";")
		else
			if Verbose then
				print("/* Macro $" .. VolMacroDefs[index].name .. " is not used, deleted. */")
			end
		end
	end
	for index = 1, #LenMacroDefs do
		local sampLen = index + 2
		if LenHistogram[SamplesPerNote * 2 + 1] ~= 0 then
			MMLFile:write("$" .. LenMacroDefs[index].name .. "=" .. LenMacroDefs[index].value .. ";")
			if LenHistogram[sampLen] == 0 then
				io.stderr:write("Macro $" .. LenMacroDefs[index].name .. " might not be used.\n")
			end
		elseif LenHistogram[sampLen] ~= 0 then
			MMLFile:write("$" .. LenMacroDefs[index].name .. "=" .. LenMacroDefs[index].value .. ";")
		else
			if Verbose then
				print("/* Macro $" .. LenMacroDefs[index].name .. " is not used, deleted. */")
			end
		end
	end
	MMLFile:write("\n")

	local mmlTempo = round(PCMInfo.samplerate * 60 / (SamplesPerNote / 4))
	if LayeredPlayback then
		MMLFile:write("$wv=")
	else
		MMLFile:write("t" .. mmlTempo)
	end
	local sampLen = 0
	local curVolume = PCM7[1]
	MMLFile:write("@e1,0,0,128,0@5@w8q16l" .. SamplesPerNote)
	MMLFile:write(MakeVolCommandAbs(curVolume))
	for sampleIndex = 1, #PCM7 do
		local nextVolume = PCM7[sampleIndex]
		if curVolume ~= nextVolume then
			MMLFile:write(MakeLenCommand(sampLen))
			sampLen = 0
		end
		sampLen = sampLen + 1
		MMLFile:write(MakeVolCommand(curVolume, nextVolume))
		curVolume = nextVolume
	end
	if sampLen > 0 then
		MMLFile:write(MakeLenCommand(sampLen))
	end

	if LayeredPlayback then
		MMLFile:write(";\n")
		MMLFile:write("t" .. mmlTempo .. "\n")
		MMLFile:write("   $wv;\n")
		for delayTick = 1, (FlMMLTimeBase / SamplesPerNote) - 1 do
			MMLFile:write("r%" .. delayTick .. "$wv;\n")
		end
	end
end

-- [ application main ] ---------------

local WhichChannel = 1

function ShowUsage()
	local options = {
		{ name = "-L, -R", description = "specify target channel (left/right)" },
		{ name = "--nolayer", description = "do not layer samples" }
	}

	print("FlPCM [options] filename.wav (filename.mml)")
	print("Convert RIFF WAVE to FlMML MML PCM (7 bit)")
	print("")
	print("Options:")
	for index = 1, #options do
		print(string.format("%s\t%s", options[index].name, options[index].description))
	end
	return
end

-- parse commandline parameters
local argi = 1
if #arg == 0 then
	ShowUsage()
	return
end
while argi <= #arg and arg[argi]:sub(1, 1) == "-" do
	if arg[argi]:sub(2, 2) == "-" then
		local thisArg = arg[argi]:sub(3)
		argi = argi + 1

		if thisArg == "histogram" then
			ShowHistogram = true
		elseif thisArg == "nolayer" then
			LayeredPlayback = false
		else
			print("Error: unknown option [--" .. thisArg .. "]")
			return
		end
	else
		local argCharIndex = 2
		local thisArg = arg[argi]:sub(2)
		argi = argi + 1

		for argCharIndex = 1, #thisArg do
			local thisArgChar = thisArg:sub(argCharIndex, argCharIndex)
			if thisArgChar == "L" then
				WhichChannel = 1
			elseif thisArgChar == "R" then
				WhichChannel = 2
			elseif thisArgChar == "v" then
				Verbose = true
			else
				print("Error: unknown option [-" .. thisArgChar .. "]")
				return
			end
		end
	end
end
if argi > #arg then
	print("Error: no input files")
	return
elseif argi + 1 < #arg then
	print("Error: too many parameters")
	return
end
local WavFilePath = arg[argi]
local MMLFile = io.stdout
if argi + 1 == #arg then
	MMLFile = io.open(arg[argi + 1], "w")
end

local PCM, PCMInfo = ReadWaveFile(WavFilePath)
if WhichChannel < 1 or WhichChannel > #PCM then
	WhichChannel = 1
end
if #PCM == 0 or #PCM[WhichChannel] == 0 then
	print("Error: Unsupported format")
	return
end

if Verbose then
	print(string.format("/* FlPCM: %d Hz, %d bit, %d channels, %d samples */",
		PCMInfo.samplerate, PCMInfo.bitwidth, PCMInfo.channels, #PCM[WhichChannel]))
end

ToFlPCM(MMLFile, PCM[WhichChannel], PCMInfo)
MMLFile:close()
if Verbose then
	print("/* Finished. */")
end
