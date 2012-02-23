#ifndef List_hpp
#define List_hpp

template <typename T>
class PtrLink
{
private:
	PtrLink(const PtrLink <T> &);
	PtrLink <T>&operator=(const PtrLink <T> &);
public:
	T *Value;
	PtrLink <T> *Next;

	PtrLink <T>(T *Value,PtrLink <T> *Next = 0) : Value(Value),Next(Next) {}

	~PtrLink()
	{
		delete Value;
		delete Next;
	}
};

template <typename T>
class PtrList
{
private:
	PtrList <T>(const PtrList <T> &);
	PtrList <T>&operator=(const PtrList <T> &);
public:
	PtrLink <T> *Front;

	PtrList() : Front(0) {}

	void Add(T *Value)
	{
		Front = new PtrLink<T>(Value,Front);
	}

	void Remove(T *Value)
	{
		PtrLink <T> *Prev = 0;
		PtrLink <T> *This = Front;
		while (This)
		{
			if (This->Value == Value)
			{
				if (Prev)
				{
					Prev->Next = This->Next;
					This->Next = 0;
					delete This;
				}
				else
				{
					Front = This->Next;
					This->Next = 0;
					delete This;
				}
				return;
			}
			Prev = This;
			This = This->Next;
		}
	}

	size_t Count()
	{
		size_t Result = 0;
		for (PtrLink <T> *it = Front;it;it = it->Next)
		{
			Result++;
		}
		return Result;
	}

	~PtrList()
	{
		delete Front;
	}
};

#endif
