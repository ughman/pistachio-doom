#ifndef IO_File_hpp
#define IO_File_hpp

#include <cstdio>

namespace IO
{
	class File
	{
	private:
		FILE *F;
		bool Writable;
	public:
		File(const char *Filename,bool Writable);

		void Read(unsigned long Position,void *Buffer,size_t Size);
		void Write(unsigned long Position,const void *Buffer,size_t Size);

		size_t Length();

		~File();
	};
}

#endif
