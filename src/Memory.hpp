#ifndef Memory_hpp
#define Memory_hpp

#include "Exception.hpp"

namespace Memory
{
	void *Allocate(size_t Size);
	void *Reallocate(void *Pointer,size_t Size);
	void Free(void *Pointer);
	void Set(void *Pointer,char Value,size_t Length);
	void Copy(void *Destination,void *Source,size_t Length);
	void Move(void *Destination,void *Source,size_t Length);

	class AllocException : public Exception
	{
	};
}

#endif
