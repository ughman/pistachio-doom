#include <cstdio>
#include <cstdlib>

#include "../Exception.hpp"
#include "MIDIStream.hpp"

Sound::MIDIStream::MIDIStream(void *UserData,unsigned char *Data,size_t Length,Vector <Sound::Synth::Instrument> Instruments,float TicksPerBeat,float BeatsPerSecond) :
Sound::Stream(UserData) ,
Instruments(Instruments) ,
TicksPerBeat(TicksPerBeat) ,
BeatsPerSecond(BeatsPerSecond)
{
	Vector <unsigned char> Buffer(Data,Length);
	while (Buffer.Length > 0)
	{
		if (Buffer[0] != 'M') throw Exception();
		if (Buffer[1] != 'T') throw Exception();
		if (Buffer[2] != 'r') throw Exception();
		if (Buffer[3] != 'k') throw Exception();
		unsigned long TrackLength;
		TrackLength  = Buffer[4] << 24;
		TrackLength |= Buffer[5] << 16;
		TrackLength |= Buffer[6] << 8;
		TrackLength |= Buffer[7];
		if (Buffer.Length < 8 + TrackLength)
		{
			throw Exception();
		}
		Tracks.Add(MIDITrack(Buffer.Array + 8,TrackLength));
		Buffer.Remove(0,8 + TrackLength);
	}
	for (Link <MIDITrack> *it = Tracks.Front;it;it = it->Next)
	{
		for (size_t i = 0;i < 16;i++)
		{
			it->Value.Channels[i].Instrument = Instruments[0];
		}
	}
}

Sound::MIDIStream::MIDIStream(void *UserData,List <MIDITrack> Tracks,Vector <Synth::Instrument> Instruments,float TicksPerBeat,float BeatsPerSecond) :
Sound::Stream(UserData) ,
Tracks(Tracks) ,
Instruments(Instruments) ,
TicksPerBeat(TicksPerBeat) ,
BeatsPerSecond(BeatsPerSecond)
{
}

size_t Sound::MIDIStream::Play(float *Output,size_t OutLength)
{
	for (size_t i = 0;i < OutLength;i++)
	{
		if (!Tracks.Front) return i;
		for (Link <MIDITrack> *it = Tracks.Front;it;)
		{
			Vector <unsigned char> &Buffer = it->Value.Buffer;
			float &TimeRemaining = it->Value.TimeRemaining;
			Sound::Synth::Channel *Channels = it->Value.Channels;
			unsigned char &Command = it->Value.Command;
			if (Buffer.Length > 0)
			{
				while (((TimeRemaining <= 0) || !it->Value.Waiting) && (Buffer.Length > 0))
				{
					if (!it->Value.Waiting)
					{
						TimeRemaining = 0;
						while (Buffer[0] & 0x80)
						{
							TimeRemaining *= 128;
							TimeRemaining += Buffer[0] & 0x7F;
							Buffer.Remove(0,1);
						}
						TimeRemaining *= 128;
						TimeRemaining += Buffer[0] & 0x7F;
						Buffer.Remove(0,1);
						it->Value.Waiting = true;
					}
					if (TimeRemaining <= 0)
					{
						if (Buffer[0] & 0x80)
						{
							Command = Buffer[0];
							Buffer.Remove(0,1);
						}
						switch (Command & 0xF0)
						{
						case 0x90:	// 0x90 before 0x80 (fall through on velocity 0)
							if (Buffer[1])
							{
								if (Command & 0xF == 9)
								{
									if (Buffer[0] >= 35 && Buffer[0] <= 81)
									{
										Channels[Command & 0xF].Instrument = Instruments[Buffer[0] - 35 + 128];
										Channels[Command & 0xF].StartNote(Buffer[0],60);
									}
								}
								else
								{
									Channels[Command & 0xF].StartNote(Buffer[0],Buffer[0]);
								}
								Buffer.Remove(0,2);
								break;
							}
							// else fall through
						case 0x80:
							{
								Channels[Command & 0xF].StopNote(Buffer[0]);
								Buffer.Remove(0,2);
							}
							break;
						case 0xA0:
							Buffer.Remove(0,2);
							break;
						case 0xB0:
							Buffer.Remove(0,2);
							break;
						case 0xC0:
							Channels[Buffer[0] & 0xF].Instrument = Instruments[Buffer[1]];
							Buffer.Remove(0,1);
							break;
						case 0xD0:
							Buffer.Remove(0,1);
							break;
						case 0xE0:
							Buffer.Remove(0,2);
							break;
						case 0xF0:
							switch (Command & 0xF)
							{
							case 0x0:
								{
									unsigned long long Length = 0;
									while (Buffer[0] & 0x80)
									{
										Length *= 128;
										Length += Buffer[0] & 0x7F;
										Buffer.Remove(0,1);
									}
									Length *= 128;
									Length += Buffer[0] & 0x7F;
									Buffer.Remove(0,1 + Length);
								}
								break;
							case 0xF:
								{
									unsigned char Type = Buffer[0];
									Buffer.Remove(0,1);
									unsigned long long Length = 0;
									while (Buffer[0] & 0x80)
									{
										Length *= 128;
										Length += Buffer[0] & 0x7F;
										Buffer.Remove(0,1);
									}
									Length *= 128;
									Length += Buffer[0] & 0x7F;
									Buffer.Remove(0,1);
									switch (Type)
									{
									case 0x51:
										{
											unsigned long Tempo;
											Tempo  = Buffer[0] << 16;
											Tempo |= Buffer[1] << 8;
											Tempo |= Buffer[2];
											BeatsPerSecond = 1 / (Tempo / 1000000.f);
										}
										break;
									}
									Buffer.Remove(0,Length);
								}
								break;
							default:
								throw Exception();
							}
							break;
						default:
							throw Exception();
						}
						it->Value.Waiting = false;
					}
				}
				for (size_t j = 0;j < 16;j++)
				{
					Channels[j].Play(Output + i,1);
				}
				TimeRemaining -= TicksPerBeat * BeatsPerSecond / 11025.f;
				it = it->Next;
			}
			else
			{
				it = it->Remove();
			}
		}
	}
	return OutLength;
}

Sound::MIDIStream *Sound::MIDIStream::Copy(void *UserData)
{
	return new Sound::MIDIStream(UserData,Tracks,Instruments,TicksPerBeat,BeatsPerSecond);
}

Sound::MIDIStream::~MIDIStream()
{
}
