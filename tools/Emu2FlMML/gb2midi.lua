-- EmuLua nes2midi script

local midipath = 'C:\\Users\\gocha\\Desktop\\gb2lua.mid' -- nil for save file popup
local mmlOutput = not false
local envVolume = 180

local MIDI = require "MIDI" -- available on LuaRocks

if not midipath then
	local savepath = input.savefilepopup()
	if not savepath[1] then
		print("MIDI conversion canceled by user.")
		return
	end
	midipath = savepath[1]
end

function round(d)
	return math.floor(d + 0.5)
end

local FlMMLTimeBase = 384

-- [ length converter ] ---------------

local LengthToNoteTable = {}
-- local LenMacroDefs = {}
function ResetLengthToNoteTable()
	local SamplesPerNote = FlMMLTimeBase
	LengthToNoteTable = {}
	-- reset
	for sampLen = 1, SamplesPerNote * 2 do
		LengthToNoteTable[sampLen] = ""
	end
	-- single note + dots
	for sampLen = 1, SamplesPerNote do
		if SamplesPerNote % sampLen == 0 then
			local baseLen = SamplesPerNote / sampLen
			LengthToNoteTable[sampLen] = "" .. baseLen

			local len = sampLen
			local dots = ""
			local dotLen = sampLen
			while dotLen % 2 == 0 do
				dotLen = dotLen / 2
				dots = dots .. "."

				len = len + dotLen
				if len <= #LengthToNoteTable then
					assert(LengthToNoteTable[len] == "")
					LengthToNoteTable[len] = baseLen .. dots
				end
			end
		end
	end
	-- tick
	for sampLen = 1, #LengthToNoteTable do
		local tick = sampLen * (FlMMLTimeBase / SamplesPerNote)
		local tickstring = "%" .. tostring(tick)
		if LengthToNoteTable[sampLen] == "" or #LengthToNoteTable[sampLen] > #tickstring then
			LengthToNoteTable[sampLen] = tickstring
		end
	end
	--[[
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
	]]
	-- macros
	--[[
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
	]]--
end

function MakeLenCommand(sampLen)
	local SamplesPerNote = FlMMLTimeBase
	local tick = sampLen * (FlMMLTimeBase / SamplesPerNote)
	local noteAbs = "%" .. tostring(tick)

	local len = sampLen
	local noteRel = ""
	--[[
	while len > SamplesPerNote * 2 do
		len = len - (SamplesPerNote * 2)
		noteRel = noteRel .. "c1c1"
	end
	noteRel = noteRel .. LengthToNoteTable[len]
	]]--

	if len <= SamplesPerNote * 2 then
		noteRel = LengthToNoteTable[len]
	end
	if len <= SamplesPerNote * 2 and #noteRel <= #noteAbs then
		return noteRel
	else
		return noteAbs
	end
end
ResetLengthToNoteTable()
-- [ asdfghjkl ] ----------------------

function score2mml(score)
	local mml = ""
	for channel, ascore in ipairs(score) do
		local eventComp = function(event1, event2)
			if event1[1] ~= event2[1] then
				-- tick sort
				return event1[1] < event2[1]
			end
			-- priority sort
			return event1[2] > event2[2]
		end
		table.sort(ascore, eventComp)

		if channel > 1 then
			mml = mml .. ";\n"
		end
		for i, event in ipairs(ascore) do
			mml = mml .. event[3]
		end
	end
	return mml
end

local miditick = 0
local snd = {
	{
		mml = { { 0, 1099, 't150' }, { 0, 1098, '@e1,0,0,' .. envVolume .. ',0' }, { 0, 1097, 'q16' } },
		score = { { 'set_tempo', 0, 1000000 }, {'track_name', 0, 'EmuLuaMIDI'}, { 'control_change', 0, 0, 91, 0 }, { 'control_change', 0, 0, 93, 0 } }
	},
	{
		mml = { { 0, 1099, '@e1,0,0,' .. envVolume .. ',0' }, { 0, 1098, 'q16' } },
		score = { { 'control_change', 0, 1, 91, 0 }, { 'control_change', 0, 1, 93, 0 } }
	},
	{
		mml = { { 0, 1098, '@e1,0,0,' .. envVolume .. ',0' }, { 0, 1097, 'q16' } },
		score = { { 'patch_change', 0, 2, 79 }, { 'control_change', 0, 2, 91, 0 }, { 'control_change', 0, 2, 93, 0 } }
	},
	{
		mml = { --[[{ 0, 1099, '@e1,0,0,' .. (envVolume * 3 / 2) .. ',0' },]] { 0, 1098, 'q16' } },
		score = { { 'patch_change', 0, 3, 115 }, { 'control_change', 0, 3, 91, 0 }, { 'control_change', 0, 3, 93, 0 } }
	}
}
for channel = 1, #snd do
	snd[channel].lastKey = -1
	snd[channel].lastVolume = -1
	snd[channel].lastVolume_ = -1
	snd[channel].lastPanpot = -1
	snd[channel].lastPanpot_ = -1
	snd[channel].lastWave = -1
	snd[channel].lastTick = 0
	snd[channel].lastTick_ = 0
	snd[channel].lastOct_ = ""
	snd[channel].lastNote_ = "r"
	snd[channel].Detune_ = 0
end

local flmmlNESDutyLookup = { 1, 2, 4, 6 }
local flmmlWaveformLookup = {}

function keyToMMLNote(key)
	if key >= 0 and key <= 127 then
		local sharpCount = 0
		if key >= 108 then
			sharpCount = key - 107
			key = 107
		end
		local sharpString = ""
		for i = 1, sharpCount do
			sharpString = sharpString .. "+"
		end
		local mmlNote = { "c", "c+", "d", "d+", "e", "f", "f+", "g", "g+", "a", "a+", "b" }
		local octave = math.floor(key / 12)
		local note = key % 12
		return "o" .. octave, mmlNote[1 + note] .. sharpString
	else
		return "", "r"
	end
end

function hexdump(binary)
	local s = ""
	for i = 1, #binary do
		s = s .. string.format("%02x", binary:byte(i))
	end
	return s
end

function framecountToMMLLength(framecount)
	return MakeLenCommand(framecount * 4)
end

function doFrameSound(snd)
	for i = 1, #snd do
		if miditick == 0 then
			snd[i].lastKey = snd[i].status.midikey
			snd[i].lastVolume = snd[i].status.volume
			snd[i].lastPanpot = snd[i].status.panpot
			snd[i].lastTick = 0
			-- snd[i].lastWave = snd[i].status.duty
		end

		local volume = round(snd[i].status.volume * 127)
		local panpot = round(snd[i].status.panpot * 126) + 1
		local lastVolume = snd[i].lastVolume
		local lastPanpot = snd[i].lastPanpot
		local key = snd[i].status.midikey
		local lastKey = snd[i].lastKey
		local waveform = snd[i].status.waveform

		local dutyChanged = (snd[i].status.duty and snd[i].status.duty ~= snd[i].lastWave)
		local waveChanged = (waveform and snd[i].lastWave ~= waveform)
		local noiseChanged = ((snd[i].status.short ~= nil) and snd[i].status.short ~= snd[i].lastWave)

		if key ~= lastKey or volume ~= lastVolume or (volume ~= 0 and 
				(panpot ~= lastPanpot or dutyChanged or waveChanged or noiseChanged)) then
			if dutyChanged then
				table.insert(snd[i].mml, { miditick, 1, "@5" })
				table.insert(snd[i].mml, { miditick, 0, "@w" .. flmmlNESDutyLookup[1 + snd[i].status.duty] })
				snd[i].lastWave = snd[i].status.duty
			end
			if noiseChanged then
				if snd[i].status.short then
					table.insert(snd[i].mml, { miditick, 1, '@e1,0,0,' .. envVolume .. ',0' })
					table.insert(snd[i].mml, { miditick, 0, "@12" })
				else
					table.insert(snd[i].mml, { miditick, 1, '@e1,0,0,' .. (envVolume * 3 / 2) .. ',0' })
					table.insert(snd[i].mml, { miditick, 0, "@11" })
				end
				snd[i].lastWave = snd[i].status.short
			end
			if waveChanged then
				if not flmmlWaveformLookup[waveform] then
					local numWaves = 0
					for k, v in pairs(flmmlWaveformLookup) do
						numWaves = numWaves + 1
					end
					flmmlWaveformLookup[waveform] = numWaves
				end
				table.insert(snd[i].mml, { miditick, 0, "@10-" .. flmmlWaveformLookup[waveform] })
				snd[i].lastWave = waveform
			end

			if volume ~= lastVolume then
				-- if lastVolume ~= 0 then
				-- 	table.insert(snd[i].mml, { snd[i].lastTick, -1000, "@x" .. round(lastVolume * 127) })
				-- end
				if volume ~= 0 and volume ~= snd[i].lastVolume_ then
					table.insert(snd[i].mml, { miditick, -999, "@x" .. volume })
					snd[i].lastVolume_ = volume
				end
				snd[i].lastVolume = volume
			end
			if panpot ~= lastPanpot then
				if panpot ~= snd[i].lastPanpot_ then
					table.insert(snd[i].mml, { miditick, -1000, "@P" .. panpot })
					snd[i].lastPanpot_ = panpot
				end
				snd[i].lastPanpot = panpot
			end
			local roundedKey = round(lastKey)
			local pitch = lastKey - roundedKey
			local octave, note = keyToMMLNote(roundedKey)
			if lastVolume == 0 or roundedKey < 0 or roundedKey > 127 then
				pitch = 0
				octave = ""
				note = "r"
			end
			if note ~= "r" then
				if i <= 3 and snd[i].Detune_ ~= round(pitch * 100) then
					table.insert(snd[i].mml, { snd[i].lastTick, -1001, "@D" .. round(pitch * 100) })
				end
				snd[i].Detune_ = round(pitch * 100)
			end
			if snd[i].lastTick ~= miditick then
				if snd[i].lastOct_ ~= octave then
					table.insert(snd[i].mml, { snd[i].lastTick, -1002, octave })
				end
				table.insert(snd[i].mml, { snd[i].lastTick, -1003, note .. framecountToMMLLength(miditick - snd[i].lastTick) })
				snd[i].lastKey = snd[i].status.midikey
				if i <= 3 and note ~= "r" and snd[i].lastNote_ ~= "r" then
					table.insert(snd[i].mml, { snd[i].lastTick_, -2000, '&' })
				end
				if note ~= "r" then
					snd[i].lastOct_ = octave
				end
				snd[i].lastNote_ = note
				snd[i].lastTick_ = snd[i].lastTick
				snd[i].lastTick = miditick
			end
		end
	end
end

emu.registerafter(function()
	local snd_ = sound.get()
	snd[1].status = snd_.square1
	snd[2].status = snd_.square2
	snd[3].status = snd_.wavememory
	snd[4].status = snd_.noise

	-- modification for midi conversion
	local flmmlGBNoiseLookup = {
		0x000002, 0x000004, 0x000008, 0x00000c, 0x000010, 0x000014, 0x000018, 0x00001c,
		0x000020, 0x000028, 0x000030, 0x000038, 0x000040, 0x000050, 0x000060, 0x000070,
		0x000080, 0x0000a0, 0x0000c0, 0x0000e0, 0x000100, 0x000140, 0x000180, 0x0001c0,
		0x000200, 0x000280, 0x000300, 0x000380, 0x000400, 0x000500, 0x000600, 0x000700,
		0x000800, 0x000a00, 0x000c00, 0x000e00, 0x001000, 0x001400, 0x001800, 0x001c00,
		0x002000, 0x002800, 0x003000, 0x003800, 0x004000, 0x005000, 0x006000, 0x007000,
		0x008000, 0x00a000, 0x00c000, 0x00e000, 0x010000, 0x014000, 0x018000, 0x01c000,
		0x020000, 0x028000, 0x030000, 0x038000, 0x040000, 0x050000, 0x060000, 0x070000
	}
	local gbNoiseMatched = false
	for i = 1, #flmmlGBNoiseLookup do
		snd[4].status.midikey = i - 1
		if snd[4].status.regs.frequency <= flmmlGBNoiseLookup[i] then
			if snd[4].status.regs.frequency == flmmlGBNoiseLookup[i] then
				gbNoiseMatched = true
			end
			break
		end
	end
	if not gbNoiseMatched then
		print("Noise not matched [".. snd[4].status.regs.frequency .. "]")
	end

	doFrameSound(snd)

	miditick = miditick + 1
end)

emu.registerexit(function()
	for waveform, index in pairs(flmmlWaveformLookup) do
		table.insert(snd[1].mml, { 0, 10000, "#WAV10 " .. index .. "," .. hexdump(waveform) .. "\n" })
	end

	local score = { 60 }
	for channel = 1, #snd do
		if snd[channel].score and snd[channel].score[1] then
			table.insert(score, snd[channel].score)
		end
	end
	local midifile = assert(io.open(midipath, 'wb'))
	midifile:write(MIDI.score2midi(score))
	midifile:close()

	if mmlOutput then
		local mml_score = {}
		for channel = 1, #snd do
			if snd[channel].mml and snd[channel].mml[1] then
				table.insert(mml_score, snd[channel].mml)
			end
		end
		local mml = score2mml(mml_score)
		if mml ~= "" then
			local mmlfile = assert(io.open(midipath .. ".mml", 'w'))
			mmlfile:write(mml)
			mmlfile:close()
		end
	end
end)
