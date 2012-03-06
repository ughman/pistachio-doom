#ifndef Vector_hpp
#define Vector_hpp

#include "Memory.hpp"
#include "Exception.hpp"

template <typename T>
class Vector
{
public:
	class BoundsException : public Exception
	{
	public:
		virtual const char *Message() const { return "Vector access out of bounds."; }

		virtual ~BoundsException() {}
	};

	T *Array;
	size_t Length;

	Vector() : Array(0),Length(0)
	{
	}

	explicit Vector(size_t Length) : Array(0),Length(Length)
	{
		Array = (T *)Memory::Allocate(sizeof(T) * Length);
	}

	explicit Vector(T *Source,size_t Length) : Array(0),Length(Length)
	{
		Array = (T *)Memory::Allocate(sizeof(T) * Length);
		using (Array)
		{
			Memory::Copy(Array,Source,sizeof(T) * Length);
		}
		end_using(Array);
	}

	Vector(const Vector &Source) : Array(0),Length(Source.Length)
	{
		Array = (T *)Memory::Allocate(sizeof(T) * Source.Length);
		using (Array)
		{
			Memory::Copy(Array,Source.Array,sizeof(T) * Source.Length);
		}
		end_using(Array);
	}

	void Add(T Value)
	{
		Array = (T *)Memory::Reallocate(Array,sizeof(T) * (Length + 1));
		Array[Length] = Value;
		Length++;
	}

	void Remove(int Start,size_t Count)
	{
		if (Start < 0 || Start + Count > Length)
		{
			throw BoundsException();
		}
		else
		{
			Memory::Move(Array + Start,Array + Start + Count,Length - Start - Count);
			Length -= Count;
			Array = (T *)Memory::Reallocate(Array,sizeof(T) * Length);
		}
	}

	T &operator[](int Offset)
	{
		if (Offset < 0 || Offset >= Length)
		{
			throw BoundsException();
		}
		else
		{
			return Array[Offset];
		}
	}

	const T &operator[](int Offset) const
	{
		if (Offset < 0 || Offset >= Length)
		{
			throw BoundsException();
		}
		else
		{
			return Array[Offset];
		}
	}

	Vector <T> &operator=(const Vector <T> &Other)
	{
		Buffer = (T *)Memory::Reallocate(Buffer,Other.Length);
		Length = Other.Length;
		Memory::Move(Buffer,Other.Buffer,Length);
		return *this;
	}

	~Vector()
	{
		Memory::Free(Array);
	}
};

#endif
