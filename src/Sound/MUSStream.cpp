#include "MUSStream.hpp"

Sound::Stream *Sound::MUSStream::Load(unsigned char *Data,size_t Length,const Vector <Sound::Synth::Instrument> &Instruments)
{
	if (Length < 16) return 0;
	if (Data[0] != 'M') return 0;
	if (Data[1] != 'U') return 0;
	if (Data[2] != 'S') return 0;
	if (Data[3] != 0x1A) return 0;
	unsigned short InstrumentCount;
	InstrumentCount  = Data[12];
	InstrumentCount |= Data[13] << 8;
	if (Length < 16 + InstrumentCount * 2) throw StrException("Invalid MUS file.");
	return new Sound::MUSStream(0,Data + 16 + InstrumentCount * 2,Length - 16 - InstrumentCount * 2,Instruments);
}

Sound::MUSStream::MUSStream(void *UserData,unsigned char *Buffer,size_t Length,const Vector <Synth::Instrument> &Instruments) :
Sound::Stream(UserData) ,
Buffer(Buffer,Length) ,
Instruments(Instruments) ,
TimeRemaining(0)
{
	for (size_t i = 0;i < 16;i++)
	{
		Channels[i].Instrument = Instruments[0];
	}
}

size_t Sound::MUSStream::Play(float *Output,size_t OutLength)
{
	for (size_t i = 0;i < OutLength;i++)
	{
		while (TimeRemaining <= 0)
		{
			bool Last = Buffer[0] & 0x80;
			int Command = (Buffer[0] & 0x70) >> 4;
			int Channel = Buffer[0] & 0xF;
			switch (Command)
			{
			case 0:
				{
					unsigned char Note = Buffer[1] & 0x7F;
					Channels[Channel].StopNote(Note);
					Buffer.Remove(0,2);
				}
				break;
			case 1:
				{
					bool WithVolume = Buffer[1] & 0x80;
					unsigned char Note = Buffer[1] & 0x7F;
					if (WithVolume)
					{
						Channels[Channel].Volume = Buffer[2] & 0x7F;
					}
					if (Channel == 15)
					{
						if (Note >= 35 && Note <= 81)
						{
							Channels[Channel].Instrument = Instruments[Note - 35 + 128];
							Channels[Channel].StartNote(Note,60);
						}
					}
					else
					{
						Channels[Channel].StartNote(Note,Note);
					}
					Buffer.Remove(0,WithVolume ? 3 : 2);
				}
				break;
			case 2:
				Buffer.Remove(0,2);
				break;
			case 3:
				Buffer.Remove(0,2);
				break;
			case 4:
				{
					unsigned char Controller = Buffer[1] & 0x7F;
					unsigned char Value = Buffer[2] & 0x7F;
					switch (Controller)
					{
					case 0:
						Channels[Channel].Instrument = Instruments[Value];
						break;
					case 3:
						Channels[Channel].Volume = Value;
						break;
					}
					Buffer.Remove(0,3);
				}
				break;
			case 6:
				return i;
			default:
				throw StrException("Unknown MUS event type.");
			}
			if (Last)
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
			}
		}
		for (size_t j = 0;j < 16;j++)
		{
			Channels[j].Play(Output + i,1);
		}
		TimeRemaining -= 140 / 11025.f;
	}
	return OutLength;
}

Sound::MUSStream *Sound::MUSStream::Copy(void *UserData)
{
	return new Sound::MUSStream(UserData,Buffer.Array,Buffer.Length,Instruments);
}

Sound::MUSStream::~MUSStream()
{
}
