-- Note: do not use vba-rr v24, it doesn't work on GB games. use v23 instead.
require("emu2midi")

function GBSoundWriter()
	local self = VGMSoundWriter()

	-- functions in base class
	self.base = ToFunctionTable(self);

	-- channel type list
	self.CHANNEL_TYPE = {
		SQUARE = "square";
		WAVEMEMORY = "wavememory";
		NOISE = "noise";
	};

	-- pseudo patch number for noise
	self.NOISE_PATCH_NUMBER = {
		LONG = 0;
		SHORT = 1;
	};

	self.FRAMERATE = 16777216 / 280896;

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

		-- search in table
		for index, targetFreq in ipairs(gbNoiseLookup) do
			if freq == targetFreq then
				return index - 1
			end
		end

		error(string.format("illegal gameboy noise frequency value 0x%06x", freq))
	end;

	-- get FlMML patch command
	-- @param string patch type (wavememory, dpcm, etc.)
	-- @param number patch number
	-- @return string patch mml text
	self.getFlMMLPatchCmd = function(self, patchType, patchNumber)
		if patchType == self.CHANNEL_TYPE.SQUARE then
			if patchNumber >= 0 and patchNumber <= 3 then
				local dutyTable = { 1, 2, 4, 6 }
				return string.format("@5@W%d", dutyTable[1 + patchNumber])
			else
				error(string.format("Unknown patch number '%d' for '%s'", patchNumber, patchType))
			end
		elseif patchType == self.CHANNEL_TYPE.WAVEMEMORY then
			return string.format("@13-%d", patchNumber)
		elseif patchType == self.CHANNEL_TYPE.NOISE then
			if patchNumber == self.NOISE_PATCH_NUMBER.LONG then
				return "@11"
			elseif patchNumber == self.NOISE_PATCH_NUMBER.SHORT then
				return "@12"
			else
				error(string.format("Unknown patch number '%d' for '%s'", patchNumber, patchType))
			end
		else
			error(string.format("Unknown patch type '%s'", patchType))
		end
	end;

	-- get FlMML waveform definition MML
	-- @return string waveform define mml
	self.getFlMMLWaveformDef = function(self)
		local mml = ""
		for waveChannelType, waveList in pairs(self.waveformList) do
			for waveIndex, waveValue in ipairs(waveList) do
				if waveChannelType == self.CHANNEL_TYPE.WAVEMEMORY then
					mml = mml .. string.format("#WAV13 %d,%s\n", waveIndex - 1, waveValue)
				else
					error(string.format("Unknown patch type '%s'", waveChannelType))
				end
			end
		end
		return mml
	end;

	self:clear()
	return self
end

local writer = GBSoundWriter()

emu.registerafter(function()
	local ch = {}
	local channels = {}
	local snd = sound.get()

	ch = snd.square1
	ch.type = writer.CHANNEL_TYPE.SQUARE
	ch.patch = ch.duty
	if ch.midikey == math.huge then -- freq reg = 0
		ch.midikey = 0
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.square2
	ch.type = writer.CHANNEL_TYPE.SQUARE
	ch.patch = ch.duty
	if ch.midikey == math.huge then -- freq reg = 0
		ch.midikey = 0
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.wavememory
	ch.type = writer.CHANNEL_TYPE.WAVEMEMORY
	ch.patch = writer.bytestohex(ch.waveform)
	if ch.midikey == math.huge then
		ch.midikey = 0 -- emulator bug? (found in NES Silver Surfer)
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.noise
	ch.type = writer.CHANNEL_TYPE.NOISE
	ch.midikey = writer.gbNoiseFreqToNote(ch.regs.frequency)
	ch.patch = (ch.short and writer.NOISE_PATCH_NUMBER.SHORT or writer.NOISE_PATCH_NUMBER.LONG)
	table.insert(channels, ch)

	writer:write(channels)
end)

_registerexit_firstrun = true
emu.registerexit(function()
	if _registerexit_firstrun then
		-- vba: without this, we will get an infinite loop on error
		_registerexit_firstrun = false

		writer:writeTextFile("testVGM.txt")
		writer:writeMidiFile("testVGM.mid")
		writer:writeFlMMLFile("testVGM.mml")
	end
end)
