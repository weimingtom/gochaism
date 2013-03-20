require("emu2midi")

function NESSoundWriter()
	local self = VGMSoundWriter()

	-- functions in base class
	self.base = ToFunctionTable(self);

	-- channel type list
	self.CHANNEL_TYPE = {
		SQUARE = "square";
		TRIANGLE = "triangle";
		NOISE = "noise";
		DPCM = "dpcm";
	};

	-- pseudo patch number for noise
	self.NOISE_PATCH_NUMBER = {
		LONG = 0;
		SHORT = 1;
	};

	self.FRAMERATE = 39375000 / 11 * 3 / 2 / 341 / 262;

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

	self:clear()
	return self
end

local writer = NESSoundWriter()

emu.registerafter(function()
	local ch = {}
	local channels = {}
	local snd = sound.get()

	ch = snd.rp2a03.square1
	ch.type = writer.CHANNEL_TYPE.SQUARE
	ch.patch = ch.duty
	if ch.midikey == math.huge then
		ch.midikey = 0 -- emulator bug? (found in NES Silver Surfer)
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.rp2a03.square2
	ch.type = writer.CHANNEL_TYPE.SQUARE
	ch.patch = ch.duty
	if ch.midikey == math.huge then
		ch.midikey = 0 -- emulator bug? (found in NES Silver Surfer)
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.rp2a03.triangle
	ch.type = writer.CHANNEL_TYPE.TRIANGLE
	ch.patch = 0
	if ch.midikey == math.huge then
		ch.midikey = 0 -- emulator bug? (found in NES Silver Surfer)
		ch.volume = 0
	end
	table.insert(channels, ch)

	ch = snd.rp2a03.noise
	ch.type = writer.CHANNEL_TYPE.NOISE
	ch.midikey = ch.regs.frequency
	ch.patch = (ch.short and writer.NOISE_PATCH_NUMBER.SHORT or writer.NOISE_PATCH_NUMBER.LONG)
	table.insert(channels, ch)

	ch = snd.rp2a03.dpcm
	ch.type = writer.CHANNEL_TYPE.DPCM
	ch.midikey = ch.regs.frequency
	ch.patch = string.format("%04x", ch.dmcaddress) -- TODO register waveform, not its address
	table.insert(channels, ch)

	writer:write(channels)
end)

_registerexit_firstrun = true
emu.registerexit(function()
	if _registerexit_firstrun then
		-- fceux: without this, we will get an error message for several times
		_registerexit_firstrun = false

		writer:writeTextFile("testVGM.txt")
		writer:writeMidiFile("testVGM.mid")
		writer:writeFlMMLFile("testVGM.mml")
	end
end)
