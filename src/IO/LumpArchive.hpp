#ifndef IO_LumpArchive_hpp
#define IO_LumpArchive_hpp

#include "File.hpp"
#include "Archive.hpp"

namespace IO
{
	class LumpArchive : public Archive
	{
	private:
		LumpArchive(const LumpArchive &);
		LumpArchive &operator=(const LumpArchive &);

		File F;
		String Filename;
	public:
		static Archive *Load(const char *Filename);

		explicit LumpArchive(const char *Filename);

		virtual unsigned int Find(const char *Name);
		virtual String ReverseFind(unsigned int Id);

		virtual void Read(unsigned int Id,void *Destination);
		virtual size_t Length(unsigned int Id);

		virtual unsigned int Count();

		virtual ~LumpArchive();
	};
}

#endif
