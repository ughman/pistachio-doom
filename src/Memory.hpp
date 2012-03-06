#ifndef Memory_hpp
#define Memory_hpp

#include "Exception.hpp"

namespace Memory
{
	void *Allocate(size_t Size);
	void *Reallocate(void *Pointer,size_t Size);
	void Free(void *Pointer);
	void Set(void *Pointer,char Value,size_t Length);
	void Copy(void *Destination,const void *Source,size_t Length);
	void Move(void *Destination,const void *Source,size_t Length);
	int Compare(const void *A,const void *B,size_t Length);

	class AllocException : public Exception
	{
	public:
		virtual const char *Message() const { return "Not enough memory."; }

		virtual ~AllocException() {}
	};
}

#endif
