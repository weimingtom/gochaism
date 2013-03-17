-- Generic MIDI/FlMML recorder abstruct class on EmuLua
-- @author gocha <http://twitter.com/gochaism>
-- @dependency http://www.pjb.com.au/comp/lua/MIDI.html

function ToFunctionTable(classObj)
	local funcTable = {}
	for k, v in pairs(classObj) do
		if type(v) == "function" then
			funcTable[k] = v
		end
	end
	return funcTable
end

function VGMSoundWriter()
	local self = {
		--[[
		class VGMSoundChannel {
			-- the following members must be given to write()
			number midikey;     -- real number, not integer
			number volume;      -- real number [0.0-1.0]
			number panpot;      -- real number [0.0-1.0], 0.5 for center
			string type;        -- type identifier, something like "square", "noise", etc.
			object patch;       -- patch identifier (number: patch number, string: patch identifier, waveform address for example)

			-- derived classes may have more additional members
		};
		]]

		FLMML_TPQN = 96;

		-- function table for derived class
		-- IMPORTANT NOTE: multiple inheritance is not allowed
		base = {};

		-- tick for new event
		tick = 0;

		-- VGMSoundChannel[] last sound state, used for logging
		lastValue = {};

		-- waveform list, which is used to record existing waveforms in a song
		-- for example: waveformList["dpcm"][1] = "3A00 (value is wavetable identifier, such as waveform address or waveform itself)";
		waveformList = {};

		-- score[], sound state log for each channels
		scoreChannel = {};
		-- score, sound state log for global things (e.g. Master Volume)
		scoreGlobal = {};

		-- pitch bend amount to consider a new note
		NOTE_PITCH_THRESHOLD = 0.25;

		-- midi note velocity value
		NOTE_VELOCITY = 100;

		-- midi pitch bend range value
		MIDI_PITCHBEND_RANGE = 12;

		-- static math.round
		-- http://lua-users.org/wiki/SimpleRound
		round = function(num, idp)
			local mult = 10^(idp or 0)
			return math.floor(num * mult + 0.5) / mult
		end;

		-- convert hex string ("414243") to byte string ("ABC")
		hextobytes = function(hexstr)
			if hexstr == nil then
				return nil
			end

			if #hexstr % 2 ~= 0 then
				error("illegal argument #1")
			end

			local bytestr = ""
			for i = 1, #hexstr, 2 do
				local s = hexstr:sub(i, i + 1)
				local n = tonumber(s, 16)
				if n == nil then
					error("illegal argument #1")
				end
				bytestr = bytestr .. string.char(n)
			end
			return bytestr
		end;

		-- convert byte string ("ABC") to hex string ("414243")
		bytestohex = function(bytestr)
			if bytestr == nil then
				return nil
			end

			local hexstr = ""
			for i = 1, #bytestr do
				hexstr = hexstr .. string.format("%02x", bytestr:byte(i))
			end
			return hexstr
		end;

		-- reset current logging state
		clear = function(self)
			self.lastValue = {}
			self.waveformList = {}
			self.scoreChannel = {}
			self.scoreGlobal = {}
			self.tick = 0
		end;

		-- add current sound state to scoreChannel, this function is called from write()
		-- @param number target channel index
		-- @param VGMSoundChannel current sound state of a channel
		addChannelState = function(self, chIndex, curr)
			if self.scoreChannel[chIndex] == nil then
				self.scoreChannel[chIndex] = {}
			end
			if self.lastValue[chIndex] == nil then
				self.lastValue[chIndex] = {}
			end

			local score = self.scoreChannel[chIndex]
			local prev = self.lastValue[chIndex]
			local channelNumber = chIndex - 1
			if curr.patch ~= prev.patch then
				local patchNumber = nil
				if type(curr.patch) == "number" then
					patchNumber = curr.patch
				else
					-- search in waveform table
					if self.waveformList[curr.type] ~= nil then
						for waveformIndex, waveform in ipairs(self.waveformList[curr.type]) do
							if patch == waveform then
								patchNumber = waveformIndex - 1
								break
							end
						end
					else
						self.waveformList[curr.type] = {}
					end
					-- add new patch if needed
					if patchNumber == nil then
						patchNumber = #self.waveformList[curr.type]
						self.waveformList[curr.type][patchNumber + 1] = curr.patch
					end
				end
				table.insert(score, { 'patch_change', self.tick, channelNumber, patchNumber, curr.type })
				prev.patch = curr.patch
			end
			if curr.volume ~= prev.volume then
				table.insert(score, { 'volume_change', self.tick, channelNumber, curr.volume })
				prev.volume = curr.volume
			end
			if curr.panpot ~= prev.panpot then
				table.insert(score, { 'panpot_change', self.tick, channelNumber, curr.panpot })
				prev.panpot = curr.panpot
			end
			if curr.midikey ~= prev.midikey then
				table.insert(score, { 'absolute_pitch_change', self.tick, channelNumber, curr.midikey })
				prev.midikey = curr.midikey
			end
		end;

		-- add current sound state to scoreGlobal, this function is called from write()
		addGlobalState = function(self)
			-- do nothing
			local score = self.scoreGlobal
		end;

		-- write current sound state to score, this function must be called every tick (frame)
		-- @param VGMSoundChannel[] current sound state of each channels
		write = function(self, channels)
			for chIndex, channel in ipairs(channels) do
				self:addChannelState(chIndex, channel)
			end
			self:addGlobalState()
			self.tick = self.tick + 1
		end;

		-- get FlMML text
		-- @return string FlMML text
		getFlMML = function(self)
			return "/* NYI */"
		end;

		-- get MIDI TPQN (integer)
		getTPQN = function(self)
			return 60
		end;

		-- get MIDI score
		-- @return string score array for MIDI.lua
		getMidiScore = function(self)
			local midiscore = {}

			-- global events
			if self.scoreGlobal then
				table.insert(midiscore, self:scoreAddEndOfTrack(self.scoreGlobal))
			end

			-- channel events
			if self.scoreChannel then
				for chIndex, score in ipairs(self.scoreChannel) do
					local channelNumber = chIndex - 1
					table.insert(score, 1, { 'control_change', 0, channelNumber, 101, 0 })
					table.insert(score, 2, { 'control_change', 0, channelNumber, 100, 0 })
					table.insert(score, 3, { 'control_change', 0, channelNumber, 6, self.MIDI_PITCHBEND_RANGE })
					table.insert(midiscore, self:scoreConvertToMidi(self:scoreBuildNote(self:scoreAddEndOfTrack(score))))
				end
			end

			return { self:getTPQN(), unpack(midiscore) }
		end;

		-- get readable text format of score (debug function)
		-- @param score target to convert
		getScoreText = function(self, score)
			local str = ""
			for i, event in ipairs(score) do
				for j, v in ipairs(event) do
					if j > 1 then
						str = str .. "\t"
					end
					str = str .. tostring(v)
				end
				str = str .. "\n"
			end
			return str
		end;

		-- write FlMML text to file
		-- @param string filename output filename
		writeFlMMLFile = function(self, filename)
			local file = assert(io.open(filename, "w"))
			file:write(self:getFlMML(self))
			file:close()
		end;

		-- write MIDI data to file
		-- @param string filename output filename
		writeMidiFile = function(self, filename)
			local MIDI = require("MIDI")
			local file = assert(io.open(filename, 'wb'))
			file:write(MIDI.score2midi(self:getMidiScore()))
			file:close()
		end;

		-- write readable text to file (debug function)
		-- @param string filename output filename
		writeTextFile = function(self, filename)
			local file = assert(io.open(filename, "w"))

			-- channel events
			for chIndex, score in ipairs(self.scoreChannel) do
				file:write(string.format("/* Channel %d */\n", chIndex - 1))

				local modscore = self:scoreConvertToMidi(self:scoreBuildNote(self:scoreAddEndOfTrack(score)))
				file:write(self:getScoreText(modscore))
				file:write("\n")
			end

			-- global events
			file:write("/* Global */\n")
			file:write(self:getScoreText(self.scoreGlobal))
			file:write("\n")

			-- wavetable table
			for waveChannelType, waveList in pairs(self.waveformList) do
				file:write(string.format("/* Waveform (%s) */\n", waveChannelType))
				for waveIndex, waveValue in ipairs(waveList) do
					file:write(waveIndex .. "\t" .. waveValue .. "\n")
				end
				file:write("\n")
			end

			file:close()
		end;

		-- event conversion: convert patch event for MIDI
		-- @param source 'patch_change' event
		eventPatchToMidi = function(self, event)
			return { { 'patch_change', event[2], event[3], event[4] } }
		end;

		-- score manipulation: add end of track
		-- @param score manipulation target
		scoreAddEndOfTrack = function(self, scoreIn)
			local score = {}
			for i, event in ipairs(scoreIn) do
				table.insert(score, event)
			end
			table.insert(score, { 'end_track', self.tick })
			return score
		end;

		-- score manipulation: build note from volume and pitch
		-- @param score manipulation target
		scoreBuildNote = function(self, scoreIn)
			-- add note off event
			local addNoteOffEvent = function(events, tick, channelNumber, noteNumber, lastEvent)
				table.insert(events, lastEvent and (#events + 1) or 1, { 'note_off', tick, channelNumber, noteNumber, 0 })
			end
			-- add note on event
			local addNoteOnEvent = function(events, tick, channelNumber, noteNumber, velocity)
				table.insert(events, { 'note_on', tick, channelNumber, noteNumber, velocity })
			end
			-- remove specified event
			local removeEvent = function(events, eventName)
				for i = #events, 1, -1 do
					local event = events[i]
					if event[1] == eventName then
						table.remove(events, i)
					end
				end
			end
			-- convert pitch bend absolute to relative
			local pitchAbsToRel = function(events, noteNumber)
				for i = #events, 1, -1 do
					local event = events[i]
					if event[1] == 'absolute_pitch_change' then
						if noteNumber ~= nil then
							local relPitch = event[4] - noteNumber
							table.remove(events, i)
							table.insert(events, i, { 'pitch_wheel_change', event[2], event[3], relPitch })
						else
							table.remove(events, i)
						end
					end
				end
			end

			local score = {}
			local eventIndex = 1
			local prev = { tick = 0 }
			local channelNumber = nil
			while eventIndex <= #scoreIn do
				local curr = {}
				local new_ = {}

				-- collect events at the same timing
				local events = { scoreIn[eventIndex] }
				curr.tick = scoreIn[eventIndex][2]
				while (eventIndex + 1) <= #scoreIn and scoreIn[eventIndex + 1][2] == curr.tick do
					eventIndex = eventIndex + 1
					table.insert(events, scoreIn[eventIndex])
				end

				-- get new volume/pitch, remove duplicated events (reverse order)
				-- channelNumber = nil
				for i = #events, 1, -1 do
					local event = events[i]
					if event[1] == 'volume_change' or event[1] == 'absolute_pitch_change' then
						if channelNumber == nil then
							channelNumber = event[3]
						else
							assert(channelNumber == event[3])
						end

						if event[1] == 'volume_change' then
							if new_.volume == nil then
								new_.volume = event[4]
							else
								table.remove(events, i)
							end
						elseif event[1] == 'absolute_pitch_change' then
							if new_.midikey == nil then
								new_.midikey = event[4]
							else
								table.remove(events, i)
							end
						end
					elseif event[1] == 'pitch_wheel_change' then
						error("relative pitch event is not supported.")
					end
				end
				-- set current volume/pitch value
				curr.volume = new_.volume or prev.volume
				curr.midikey = new_.midikey or prev.midikey
				curr.noteNumber = prev.noteNumber

				if curr.volume and curr.volume ~= 0 then
					if prev.volume and prev.volume ~= 0 then
						local pitchDiff = math.abs(curr.midikey - prev.midikey)
						if pitchDiff > 0 then
							if pitchDiff >= self.NOTE_PITCH_THRESHOLD then
								curr.noteNumber = self.round(curr.midikey)
								if curr.noteNumber ~= prev.noteNumber then
									-- new note! (frequency changed)
									addNoteOffEvent(events, curr.tick, channelNumber, prev.noteNumber)
									addNoteOnEvent(events, curr.tick, channelNumber, curr.noteNumber, self.NOTE_VELOCITY)
									-- removeEvent(events, 'absolute_pitch_change')
									prev.noteNumber = curr.noteNumber
								end
							end
						end
						prev.midikey = curr.midikey
					else
						-- new note! (from volume 0)
						curr.noteNumber = self.round(curr.midikey)
						addNoteOnEvent(events, curr.tick, channelNumber, curr.noteNumber, self.NOTE_VELOCITY)
						-- removeEvent(events, 'absolute_pitch_change')
						prev.noteNumber = curr.noteNumber
					end
				else
					if prev.volume and prev.volume ~= 0 then
						-- end of note
						addNoteOffEvent(events, curr.tick, channelNumber, prev.noteNumber)
						removeEvent(events, 'volume_change')
						prev.noteNumber = nil
					else
						-- no sound / rest before the first note
					end
				end
				prev.midikey = curr.midikey
				prev.volume = curr.volume

				-- convert pitch bend absolute to relative
				pitchAbsToRel(events, prev.noteNumber)

				-- finally...
				if eventIndex == #scoreIn then
					-- add missing note off
					if prev.noteNumber then
						addNoteOffEvent(events, curr.tick, channelNumber, prev.noteNumber)
						prev.noteNumber = nil
					end
				end

				-- copy the modified events to output score
				for i, event in ipairs(events) do
					table.insert(score, event)
				end

				eventIndex = eventIndex + 1
			end
			return score
		end;

		-- score manipulation: convert to MIDI compatible event
		-- @param score manipulation target
		scoreConvertToMidi = function(self, scoreIn)
			local score = {}
			for i, event in ipairs(scoreIn) do
				if event[1] == 'patch_change' then
					local patchEvents = self:eventPatchToMidi(event)
					for j, patchEvent in ipairs(patchEvents) do
						table.insert(score, patchEvent)
					end
				elseif event[1] == 'volume_change' then
					local value = event[4]
					assert(value >= 0.0 and value <= 1.0)

					-- gain[dB] = 40 * log10(cc7/127) 
					value = math.sqrt(value)

					event = { 'control_change', event[2], event[3], 7, self.round(value * 127) }
					table.insert(score, event)
				elseif event[1] == 'panpot_change' then
					local value = event[4]
					assert(value >= 0.0 and value <= 1.0)

					-- GM2 recommended formula:
					-- Left Channel Gain [dB] = 20*log(cos(PI/2*max(0,cc#10-1)/126)) 
					-- Right Channel Gain [dB] = 20*log(sin(PI/2*max(0,cc#10-1)/126))
					-- TODO: decent panpot curve

					event = { 'control_change', event[2], event[3], 10, self.round(value * 126) + 1 }
					table.insert(score, event)
				elseif event[1] == 'pitch_wheel_change' then
					local value = event[4]

					if value < -self.MIDI_PITCHBEND_RANGE or value > self.MIDI_PITCHBEND_RANGE then
						print(string.format("Warning: pitch bend range overflow <%f> cent at tick <%d> channel <%d>.", value * 100, event[2], event[3]))
						value = math.min(self.MIDI_PITCHBEND_RANGE, math.max(-self.MIDI_PITCHBEND_RANGE, value))
					end

					event = { 'pitch_wheel_change', event[2], event[3], math.min(self.round(value / self.MIDI_PITCHBEND_RANGE * 8192), 8191) }
					table.insert(score, event)
				elseif event[1] == 'absolute_pitch_change' then
					error("'absolute_pitch_change' need to be converted before scoreConvertToMidi.")
				else
					table.insert(score, event)
				end
			end
			return score
		end;
	}

	self:clear()
	return self
end
