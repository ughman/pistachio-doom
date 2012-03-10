#ifndef IO_MultiArchive_hpp
#define IO_MultiArchive_hpp

#include "../List.hpp"
#include "Archive.hpp"

namespace IO
{
	class MultiArchive : public Archive
	{
	private:
		MultiArchive(const MultiArchive &);
		MultiArchive &operator=(const MultiArchive &);

		PtrList <Archive> Archives;
	public:
		MultiArchive();

		virtual unsigned int Find(const char *Name);
		virtual String ReverseFind(unsigned int Id);

		virtual void Read(unsigned int Id,void *Destination);
		virtual size_t Length(unsigned int Id);

		virtual unsigned int Count();

		void Add(Archive *Archive);

		virtual ~MultiArchive();
	};
}

#endif
