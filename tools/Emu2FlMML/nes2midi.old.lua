-- EmuLua nes2midi script
-- TODO: auto DPCM mapping

local midipath = 'C:\\Users\\gocha\\Desktop\\fceuxlua.mid' -- nil for save file popup
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
		mml = { { 0, 1099, '@6' }, { 0, 1098, '@e1,0,0,' .. envVolume .. ',0' }, { 0, 1097, 'q16' } },
		score = { { 'patch_change', 0, 2, 79 }, { 'control_change', 0, 2, 91, 0 }, { 'control_change', 0, 2, 93, 0 } }
	},
	{
		mml = { { 0, 1099, '@e1,0,0,' .. round(envVolume * 2 / 3) .. ',0' }, { 0, 1098, 'q16' } },
		score = { { 'patch_change', 0, 3, 115 }, { 'control_change', 0, 3, 91, 0 }, { 'control_change', 0, 3, 93, 0 } }
	},
	{
		mml = { { 0, 1099, '@e1,0,0,' .. round(envVolume * 2.5) .. ',0' }, { 0, 1098, 'q16' } },
		score = { { 'patch_change', 0, 4, 116 }, { 'control_change', 0, 4, 91, 0 }, { 'control_change', 0, 4, 93, 0 } }
	}
}
for channel = 1, #snd do
	snd[channel].lastKey = -1
	snd[channel].lastVolume = -1
	snd[channel].lastVolume_ = -1
	snd[channel].lastDuty = -1
	snd[channel].lastTick = 0
	snd[channel].lastTick_ = 0
	snd[channel].lastOct_ = ""
	snd[channel].lastNote_ = "r"
	snd[channel].Detune_ = 0
end

local flmmlNESDutyLookup = { 1, 2, 4, 6 }

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

function framecountToMMLLength(framecount)
	return MakeLenCommand(framecount * 4)
end

function doFrameSound(snd)
	for i = 1, #snd do
		if miditick == 0 then
			snd[i].lastKey = snd[i].status.midikey
			snd[i].lastVolume = snd[i].status.volume
			snd[i].lastTick = 0
			-- snd[i].lastDuty = snd[i].status.duty
		end

		local volume = snd[i].status.volume
		local lastVolume = snd[i].lastVolume
		local key = snd[i].status.midikey
		local lastKey = snd[i].lastKey

		-- TODO: duty change must be done just before a new note
		if snd[i].status.duty and snd[i].status.duty ~= snd[i].lastDuty then
			table.insert(snd[i].mml, { miditick, 1, "@5" })
			table.insert(snd[i].mml, { miditick, 0, "@w" .. flmmlNESDutyLookup[1 + snd[i].status.duty] })
			snd[i].lastDuty = snd[i].status.duty
		end
		if (snd[i].status.short ~= nil) and snd[i].status.short ~= snd[i].lastDuty then
			if snd[i].status.short then
				table.insert(snd[i].mml, { miditick, 0, "@8" })
			else
				table.insert(snd[i].mml, { miditick, 0, "@7" })
			end
			snd[i].lastDuty = snd[i].status.short
		end
		if (i ~= 5 and key ~= lastKey) or volume ~= lastVolume then
			if volume ~= lastVolume then
				-- if lastVolume ~= 0 then
				-- 	table.insert(snd[i].mml, { snd[i].lastTick, -1000, "@x" .. round(lastVolume * 127) })
				-- end
				if volume ~= 0 and volume ~= snd[i].lastVolume_ then
					table.insert(snd[i].mml, { miditick, -1000, "@x" .. round(volume * 127) })
					snd[i].lastVolume_ = volume
				end
				snd[i].lastVolume = snd[i].status.volume
			end
			local roundedKey = round(lastKey)
			local pitch = lastKey - roundedKey
			local octave, note = keyToMMLNote(roundedKey)
			if lastVolume == 0 or roundedKey < 0 or roundedKey > 127 then
				pitch = 0
				octave = ""
				note = "r"
			end
			if i == 5 and lastVolume ~= 0 then
				local dmcHex = ""
				local dmcBin = ""
				for addr = snd[i].status.dmcaddress, snd[i].status.dmcaddress + snd[i].status.dmcsize do
					local d = memory.readbyte(addr)
					dmcHex = dmcHex .. string.format("%02x", d)
					dmcBin = dmcBin .. string.char(d)
				end
				note = string.format("/*@%04X:%02X %d,%d %s*/" .. note, snd[i].status.dmcaddress, snd[i].status.dmcsize, snd[i].status.dmcseed, (snd[i].status.dmcloop and 1 or 0), dmcHex)
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
	snd[1].status = snd_.rp2a03.square1
	snd[2].status = snd_.rp2a03.square2
	snd[3].status = snd_.rp2a03.triangle
	snd[4].status = snd_.rp2a03.noise
	snd[5].status = snd_.rp2a03.dpcm

	-- modification for midi conversion
	snd[4].status.midikey = snd[4].status.regs.frequency
	snd[5].status.midikey = snd[5].status.regs.frequency -- math.floor((snd[5].status.dmcaddress - 0xC000) / 0x40)

	doFrameSound(snd)

	miditick = miditick + 1
end)

emu.registerexit(function()
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
