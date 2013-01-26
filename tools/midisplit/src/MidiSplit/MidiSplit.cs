using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Text;
using System.IO;
using CannedBytes.Media.IO;
using CannedBytes.Midi;
using CannedBytes.Midi.IO;
using CannedBytes.Midi.Message;

namespace MidiSplit
{
    public class MidiSplit
    {
        public static int Main(string[] args)
        {
            try
            {
                string midiInFilePath = null;
                string midiOutFilePath = null;

                // parse argument
                if (args.Length == 0)
                {
                    Console.WriteLine("Usage: MidiSplit input.mid output.mid");
                    return 1;
                }
                else if (args.Length == 1)
                {
                    midiInFilePath = args[0];
                    midiOutFilePath = Path.Combine(
                        Path.GetDirectoryName(midiInFilePath),
                        Path.GetFileNameWithoutExtension(midiInFilePath) + "-split.mid"
                    );
                }
                else if (args.Length == 2)
                {
                    midiInFilePath = args[0];
                    midiOutFilePath = args[1];
                }
                else
                {
                    Console.WriteLine("too many arguments");
                    return 1;
                }

                // for debug...
                Console.WriteLine("Reading midi file: " + midiInFilePath);
                Console.WriteLine("Output midi file: " + midiOutFilePath);

                MidiFileData midiInData = MidiSplit.ReadMidiFile(midiInFilePath);
                MidiFileData midiOutData = MidiSplit.SplitMidiFile(midiInData);
                using (MidiFileSerializer midiFileSerializer = new MidiFileSerializer(midiOutFilePath))
                {
                    midiFileSerializer.Serialize(midiOutData);
                }
                return 0;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return 1;
            }
        }

        static MidiFileData SplitMidiFile(MidiFileData midiInData)
        {
            MidiFileData midiOutData = new MidiFileData();
            midiOutData.Header = new MThdChunk();
            midiOutData.Header.Format = (ushort)MidiFileFormat.MultipleTracks;
            midiOutData.Header.TimeDivision = midiInData.Header.TimeDivision;

            IList<MTrkChunk> tracks = new List<MTrkChunk>();
            foreach (MTrkChunk midiTrackIn in midiInData.Tracks)
            {
                foreach (MTrkChunk midiTrackOut in MidiSplit.SplitMidiTrack(midiTrackIn))
                {
                    tracks.Add(midiTrackOut);
                }
            }
            midiOutData.Tracks = tracks;
            midiOutData.Header.NumberOfTracks = (ushort)tracks.Count;
            return midiOutData;
        }

        protected class MTrkChunkWithInstrInfo
        {
            public int? MidiChannel;
            public int? ProgramNumber;
            public MTrkChunk Track;
        }

        static IEnumerable<MTrkChunk> SplitMidiTrack(MTrkChunk midiTrackIn)
        {
            const int MaxChannels = 16;
            int?[] currentProgramNumber = new int?[MaxChannels];
            IList<MTrkChunkWithInstrInfo> trackInfos = new List<MTrkChunkWithInstrInfo>();
            
            // create default output track
            trackInfos.Add(new MTrkChunkWithInstrInfo());
            trackInfos[0].Track = new MTrkChunk();
            trackInfos[0].Track.Events = new List<MidiFileEvent>();

            // dispatch events from beginning
            // (events must be sorted by absolute time)
            MidiFileEvent midiLastEvent = null;
            foreach (MidiFileEvent midiEvent in midiTrackIn.Events)
            {
                MTrkChunk targetTrack = null;

                // save the last event to verify end of track
                midiLastEvent = midiEvent;

                if (midiEvent.Message is MidiChannelMessage)
                {
                    MidiChannelMessage channelMessage = midiEvent.Message as MidiChannelMessage;

                    // if this is the first channel message
                    if (trackInfos[0].MidiChannel == null)
                    {
                        // set the channel number to the first track
                        trackInfos[0].MidiChannel = channelMessage.MidiChannel;
                    }

                    // update current patch #
                    if (channelMessage.Command == MidiChannelCommand.ProgramChange)
                    {
                        currentProgramNumber[channelMessage.MidiChannel] = channelMessage.Parameter1;
                    }

                    // search target track
                    int trackIndex;
                    for (trackIndex = 0; trackIndex < trackInfos.Count; trackIndex++)
                    {
                        MTrkChunkWithInstrInfo trackInfo = trackInfos[trackIndex];

                        if (trackInfo.MidiChannel == channelMessage.MidiChannel &&
                            (trackInfo.ProgramNumber == null ||
                              trackInfo.ProgramNumber == currentProgramNumber[channelMessage.MidiChannel]))
                        {
                            // set program number (we need to set it for the first time)
                            trackInfo.ProgramNumber = currentProgramNumber[channelMessage.MidiChannel];

                            // target track is determined, exit the loop
                            targetTrack = trackInfo.Track;
                            break;
                        }
                        else if (trackInfo.MidiChannel > channelMessage.MidiChannel)
                        {
                            // track list is sorted by channel number
                            // therefore, the rest isn't what we are searching for
                            // a new track needs to be assigned to the current index
                            break;
                        }
                    }

                    // add a new track if necessary
                    if (targetTrack == null)
                    {
                        MTrkChunkWithInstrInfo newTrackInfo = new MTrkChunkWithInstrInfo();
                        newTrackInfo.Track = new MTrkChunk();
                        newTrackInfo.Track.Events = new List<MidiFileEvent>();
                        newTrackInfo.MidiChannel = channelMessage.MidiChannel;
                        newTrackInfo.ProgramNumber = currentProgramNumber[channelMessage.MidiChannel];
                        trackInfos.Insert(trackIndex, newTrackInfo);
                        targetTrack = newTrackInfo.Track;
                    }
                }
                else
                {
                    targetTrack = trackInfos[0].Track;
                }

                IList<MidiFileEvent> targetEventList = targetTrack.Events as IList<MidiFileEvent>;
                targetEventList.Add(midiEvent);
            }

            // check end of track and save its location
            long? trackEndAbsoluteTime = null;
            if (midiLastEvent != null && midiLastEvent.Message is MidiMetaMessage &&
                (midiLastEvent.Message as MidiMetaMessage).MetaType == MidiMetaType.EndOfTrack)
            {
                trackEndAbsoluteTime = midiLastEvent.AbsoluteTime;
            }

            // construct the track list without extra info
            IList<MTrkChunk> tracks = new List<MTrkChunk>();
            foreach (MTrkChunkWithInstrInfo trackInfo in trackInfos)
            {
                Console.WriteLine("Track for Channel " + trackInfo.MidiChannel + " Program " + trackInfo.ProgramNumber + "");
                tracks.Add(trackInfo.Track);
            }

            // fixup delta time artifically...
            foreach (MTrkChunk track in tracks)
            { 
                midiLastEvent = null;
                foreach (MidiFileEvent midiEvent in track.Events)
                {
                    midiEvent.DeltaTime = midiEvent.AbsoluteTime - (midiLastEvent != null ? midiLastEvent.AbsoluteTime : 0);
                    midiLastEvent = midiEvent;
                }

                // add missing end of track
                if (midiLastEvent == null || !(midiLastEvent.Message is MidiMetaMessage) ||
                    (midiLastEvent.Message as MidiMetaMessage).MetaType != MidiMetaType.EndOfTrack)
                {
                    long absoluteTime = midiLastEvent.AbsoluteTime;
                    if (trackEndAbsoluteTime != null && absoluteTime < trackEndAbsoluteTime)
                    {
                        absoluteTime = trackEndAbsoluteTime.Value;
                    }

                    MidiFileEvent endOfTrack = new MidiFileEvent();
                    endOfTrack.AbsoluteTime = absoluteTime;
                    endOfTrack.DeltaTime = absoluteTime - midiLastEvent.AbsoluteTime;
                    endOfTrack.Message = new MidiMetaMessage(MidiMetaType.EndOfTrack, new byte[] { });
                    (track.Events as IList<MidiFileEvent>).Add(endOfTrack);
                }

                foreach (MidiFileEvent midiEvent in track.Events)
                {    
                    Console.WriteLine("" + midiEvent.AbsoluteTime + "\t" + midiEvent.DeltaTime + "\t" + midiEvent.Message.ToString() + "\t" +
                        hexstring(midiEvent.Message.GetData()));

                }


                Console.WriteLine("--------------------------------------------");
            }

            return tracks;
        }

        private static String hexstring(byte[] b)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < b.Length; i++)
            {
                sb.Append(String.Format("{0,0:X2} ", b[i]));
            }
            return sb.ToString();
        }

        static MidiFileData ReadMidiFile(string filePath)
        {
            MidiFileData data = new MidiFileData();
            FileChunkReader reader = FileReaderFactory.CreateReader(filePath);

            data.Header = reader.ReadNextChunk() as MThdChunk;

            List<MTrkChunk> tracks = new List<MTrkChunk>();

            for (int i = 0; i < data.Header.NumberOfTracks; i++)
            {
                try
                {
                    var track = reader.ReadNextChunk() as MTrkChunk;

                    if (track != null)
                    {
                        tracks.Add(track);
                    }
                    else
                    {
                        Console.WriteLine(String.Format("Track '{0}' was not read successfully.", i + 1));
                    }
                }
                catch (Exception e)
                {
                    reader.SkipCurrentChunk();

                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("Failed to read track: " + (i + 1));
                    Console.WriteLine(e);
                    Console.ForegroundColor = ConsoleColor.Gray;
                }
            }

            data.Tracks = tracks;
            return data;
        }

/*            IEnumerable<MidiFileEvent> notes = null;

            // merge all track notes and filter out sysex and meta events
            foreach (var track in fileData.Tracks)
            {
                if (notes == null)
                {
                    notes = from note in track.Events
                            where !(note.Message is MidiLongMessage)
                            select note;
                }
                else
                {
                    notes = (from note in track.Events
                             where !(note.Message is MidiLongMessage)
                             select note).Union(notes);
                }
            }

            // order track notes by absolute-time.
            notes = from note in notes
                    orderby note.AbsoluteTime
                    select note;

            // At this point the DeltaTime properties are invalid because other events from other
            // tracks are now merged between notes where the initial delta-time was calculated for.
            // We fix this in the play back routine.
 */
/*
            WriteHeaderInfoToConsole(fileData.Header);

            int trackIndex = 1;
            foreach (MTrkChunk track in fileData.Tracks)
            {
                Console.WriteLine("Track " + trackIndex);
                foreach (MidiFileEvent midiEvent in track.Events)
                {
                    String attribute = "?";
                    if (midiEvent.Message is MidiShortMessage)
                    {
                        MidiShortMessage midiMessage = midiEvent.Message as MidiShortMessage;
                        attribute = "S";
                        Console.WriteLine("" + midiEvent.AbsoluteTime + "\t" + midiMessage.ByteLength + "\t" + attribute + "\t" +
                            midiMessage.GetType() + "\t" + ToHexString(midiMessage.GetData()));
                    }
                    else if (midiEvent.Message is MidiLongMessage)
                    {
                        MidiLongMessage midiMessage = midiEvent.Message as MidiLongMessage;
                        attribute = "L";
                        Console.WriteLine("" + midiEvent.AbsoluteTime + "\t" + midiMessage.ByteLength + "\t" + attribute + "\t" +
                            midiMessage.GetType() + "\t" + ToHexString(midiMessage.GetData()));
                    }
                }
                trackIndex++;
            }
            */
            /*
            var caps = MidiOutPort.GetPortCapabilities(outPortId);
            MidiOutPortBase outPort = null;

            try
            {
                outPort = ProcessStreaming(outPortId, fileData, notes, caps);
            }
            catch (Exception e)
            {
                Console.WriteLine();

                Console.WriteLine(e.ToString());
            }
*/
//            Console.WriteLine("Press any key to exit...");
//            Console.ReadKey();

            /*
            if (outPort != null)
            {
                outPort.Reset();
                if (!outPort.BufferManager.WaitForBuffersReturned(1000))
                {
                    Console.WriteLine("Buffers failed to return in 1 sec.");
                }

                outPort.Close();
                outPort.Dispose();
            }
             * */

        /*
        private static MidiOutPortBase ProcessStreaming(int outPortId, MidiFileData fileData,
            IEnumerable<MidiFileEvent> notes, MidiOutPortCaps caps)
        {
            var outPort = new MidiOutStreamPort();
            outPort.Open(outPortId);
            outPort.BufferManager.Initialize(10, 1024);
            outPort.TimeDivision = fileData.Header.TimeDivision;

            // TODO: extract Tempo from meta messages from the file.
            // 120 bpm (uSec/QuarterNote).
            outPort.Tempo = 500000;

            Console.WriteLine(String.Format("Midi Out Stream Port '{0}' is now open.", caps.Name));

            MidiMessageOutStreamWriter writer = null;
            MidiBufferStream buffer = null;
            MidiFileEvent lastNote = null;

            foreach (var note in notes)
            {
                if (writer == null)
                {
                    // brute force buffer aqcuirement.
                    // when callbacks are implemented this will be more elegant.
                    do
                    {
                        buffer = outPort.BufferManager.RetrieveBuffer();

                        if (buffer != null) break;

                        Thread.Sleep(50);
                    } while (buffer == null);

                    writer = new MidiMessageOutStreamWriter(buffer);
                }

                if (writer.CanWrite(note.Message))
                {
                    if (lastNote != null)
                    {
                        // fixup delta time artifically...
                        writer.Write(note.Message, (int)(note.AbsoluteTime - lastNote.AbsoluteTime));
                    }
                    else
                    {
                        writer.Write(note.Message, (int)note.DeltaTime);
                    }
                }
                else
                {
                    outPort.LongData(buffer);
                    writer = null;

                    Console.WriteLine("Buffer sent...");

                    if (!outPort.HasStatus(MidiPortStatus.Started))
                    {
                        outPort.Restart();
                    }
                }

                lastNote = note;
            }

            return outPort;
        }
         */

/*        private static string ToHexString(byte[] bytes)
        {
            StringBuilder sb = new StringBuilder(bytes.Length * 2);
            foreach (byte b in bytes)
            {
                if (b < 16) sb.Append('0');
                sb.Append(Convert.ToString(b, 16).ToUpper() + " ");
            }
            return sb.ToString();
        }

        private static void WriteHeaderInfoToConsole(MThdChunk mThdChunk)
        {
            Console.WriteLine("Number of tracks: " + mThdChunk.NumberOfTracks);
            Console.WriteLine("Number of format: " + mThdChunk.Format);
            Console.WriteLine("Number of time division: " + mThdChunk.TimeDivision);
        }*/
    }
}