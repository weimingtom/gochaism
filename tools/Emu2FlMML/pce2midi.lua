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

	-- get FlMML patch command
	-- @param string patch type (wavememory, dpcm, etc.)
	-- @param number patch number
	-- @return string patch mml text
	self.getFlMMLPatchCmd = function(self, patchType, patchNumber)
		if patchType == self.CHANNEL_TYPE.WAVEMEMORY then
			return string.format("@13-%d", patchNumber)
		elseif patchType == self.CHANNEL_TYPE.NOISE then
			return "@11"
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

	-- get FlMML tuning for each patches
	-- @param number origNoteNumber input note number
	-- @param string patchType patch type (square, noise, etc.)
	-- @return number output note number
	self.getFlMMLNoteNumber = function(self, origNoteNumber, patchType)
		if patchType == self.CHANNEL_TYPE.NOISE then
			-- convert to gameboy noise
			assert(origNoteNumber >= 0 and origNoteNumber <= 31)
			return self.gbNoiseFreqToNote(1048576.0 / origNoteNumber)
		else
			return origNoteNumber
		end
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
			if ch.midikey == math.huge then -- freq reg = 0
				ch.midikey = 0
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
