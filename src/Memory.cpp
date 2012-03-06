#include <cstdlib>
#include <cstring>

#include "Memory.hpp"

void *Memory::Allocate(size_t Size)
{
	void *Result = malloc(Size);
	if (Result)
	{
		return Result;
	}
	else
	{
		throw AllocException();
	}
}

void *Memory::Reallocate(void *Pointer,size_t Size)
{
	if (Size == 0)
	{
		free(Pointer);
		return 0;
	}
	else
	{
		void *Result = realloc(Pointer,Size);
		if (Result)
		{
			return Result;
		}
		else
		{
			throw AllocException();
		}
	}
}

void Memory::Free(void *Pointer)
{
	free(Pointer);
}

void Memory::Set(void *Pointer,char Value,size_t Length)
{
	memset(Pointer,Value,Length);
}

void Memory::Copy(void *Destination,const void *Source,size_t Length)
{
	memcpy(Destination,Source,Length);
}

void Memory::Move(void *Destination,const void *Source,size_t Length)
{
	memmove(Destination,Source,Length);
}

int Memory::Compare(const void *A,const void *B,size_t Length)
{
	return memcmp(A,B,Length);
}

void *operator new(size_t Size)
{
	return Memory::Allocate(Size);
}

void *operator new[](size_t Size)
{
	return Memory::Allocate(Size);
}

void operator delete(void *Pointer)
{
	Memory::Free(Pointer);
}

void operator delete[](void *Pointer)
{
	Memory::Free(Pointer);
}
