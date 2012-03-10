#ifndef IO_WADArchive_hpp
#define IO_WADArchive_hpp

#include "File.hpp"
#include "Archive.hpp"

namespace IO
{
	class WADArchive : public Archive
	{
	private:
		WADArchive(const WADArchive &);
		WADArchive &operator=(const WADArchive &);

		File F;
		unsigned long LumpCount;
		unsigned long LumpTableOffset;
	public:
		static Archive *Load(const char *Filename);

		explicit WADArchive(const char *Filename);

		virtual unsigned int Find(const char *Name);
		virtual String ReverseFind(unsigned int Id);

		virtual void Read(unsigned int Id,void *Destination);
		virtual size_t Length(unsigned int Id);

		virtual unsigned int Count();

		virtual ~WADArchive();
	};
}

#endif
