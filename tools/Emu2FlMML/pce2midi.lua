require("emu2midi")

function PCESoundWriter()
	local self = VGMSoundWriter()

	-- functions in base class
	self.base = ToFunctionTable(self);

	-- channel type list
	self.CHANNEL_TYPE = {
		WAVEMEMORY = "wavememory";
		NOISE = "noise";
	};

	self.FRAMERATE = (21477272 + (72/99)) / 3 / 455 / 263; -- 72/99=0.7272...

	-- reset current logging state
	self.clear = function(self)
		self.base.clear(self)

		-- tempo (fixed value)
		local bpm = self.FRAMERATE
		table.insert(self.scoreGlobal, { 'set_tempo', 0, math.floor(60000000 / bpm) })
	end;

	-- get MIDI TPQN (integer)
	self.getTPQN = function(self)
		return 60
	end;

	-- event conversion: convert patch event for MIDI
	-- @param source 'patch_change' event
	self.eventPatchToMidi = function(self, event)
		return { { 'patch_change', event[2], event[3], event[4] } }
	end;

	-- static pceNoiseRegToFreq
	-- @param number noise frequency register value (0-31)
	-- @return number noise frequency [Hz]
	self.pceNoiseRegToFreq = function(reg)
		assert(reg >= 0 and reg <= 0x1f)
		return 3579545.0 / reg
	end;

	-- static gbNoiseFreqToNote
	-- @param number noise frequency register value
	-- @return number FlMML compatible noise note number
	self.gbNoiseFreqToNote = function(freq)
		local gbNoiseLookup = {
			0x000002, 0x000004, 0x000008, 0x00000c, 0x000010, 0x000014, 0x000018, 0x00001c,
			0x000020, 0x000028, 0x000030, 0x000038, 0x000040, 0x000050, 0x000060, 0x000070,
			0x000080, 0x0000a0, 0x0000c0, 0x0000e0, 0x000100, 0x000140, 0x000180, 0x0001c0,
			0x000200, 0x000280, 0x000300, 0x000380, 0x000400, 0x000500, 0x000600, 0x000700,
			0x000800, 0x000a00, 0x000c00, 0x000e00, 0x001000, 0x001400, 0x001800, 0x001c00,
			0x002000, 0x002800, 0x003000, 0x003800, 0x004000, 0x005000, 0x006000, 0x007000,
			0x008000, 0x00a000, 0x00c000, 0x00e000, 0x010000, 0x014000, 0x018000, 0x01c000,
			0x020000, 0x028000, 0x030000, 0x038000, 0x040000, 0x050000, 0x060000, 0x070000
		}

		-- search in table (search the nearest one)
		local bestDiff = math.huge
		local bestIndex = 1
		for index, targetFreq in ipairs(gbNoiseLookup) do
			local diff = math.abs(freq - targetFreq)
			if diff < bestDiff then
				bestIndex = index
				bestDiff = diff
			end
		end

		return bestIndex - 1
	end;

	-- convert 5bit wave to 8bit wave
	self.byte5bitTo8bit = function(bytestring)
		if bytestring == nil then
			return nil
		end

		local str = ""
		assert(#bytestring == 32)
		for i = 1, #bytestring do
			local raw5 = string.byte(bytestring, i)
			assert(raw5 >= 0 and raw5 <= 31)
			local raw8 = (raw5 * 8) -- + math.floor(raw5 / 4)
			str = str .. string.char(raw8)
		end
		return str
	end;

	self:clear()
	return self
end

local writer = PCESoundWriter()

emu.registerafter(function()
	local ch = {}
	local channels = {}
	local snd = sound.get()

	for chIndex = 1, #snd.channel do
		ch = snd.channel[chIndex]
		if ch.noise then
			ch.type = writer.CHANNEL_TYPE.NOISE
			ch.midikey = ch.noise
			ch.patch = 127
		else
			ch.type = writer.CHANNEL_TYPE.WAVEMEMORY
			-- TODO: handle ch.dda
			ch.patch = writer.bytestohex(writer.byte5bitTo8bit(ch.waveform))
			if ch.midikey == math.huge then
				ch.midikey = 0 -- emulator bug? (found in NES Silver Surfer)
				ch.volume = 0
			end
		end
		table.insert(channels, ch)
	end

	writer:write(channels)
end)

emu.registerexit(function()
	writer:writeTextFile("testVGM.txt")
	writer:writeMidiFile("testVGM.mid")
	writer:writeFlMMLFile("testVGM.mml")
end)
