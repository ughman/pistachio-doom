#ifndef IO_Archive_hpp
#define IO_Archive_hpp

#include "../List.hpp"
#include "../String.hpp"
#include "../Exception.hpp"

namespace IO
{
	class Archive
	{
	private:
		Archive(const Archive &);
		Archive &operator=(const Archive &);
	public:
		class EntryNotFoundException : public Exception
		{
		public:
			EntryNotFoundException() {}

			virtual const char *Message() const { return "Entry not found."; }

			virtual ~EntryNotFoundException() {}
		};

		typedef Archive *(*Format)(const char *Filename);

		static List <Format> Formats;

		static Archive *Load(const char *Filename);

		Archive() {}

		virtual unsigned int Find(const char *Name) = 0;
		virtual String ReverseFind(unsigned int Id) = 0;

		virtual void Read(unsigned int Id,void *Destination) = 0;
		virtual size_t Length(unsigned int Id) = 0;

		virtual unsigned int Count() = 0;

		virtual ~Archive() {}
	};
}

#endif
