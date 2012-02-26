#ifndef List_hpp
#define List_hpp

template <typename T>
class PtrList;

template <typename T>
class PtrLink
{
private:
	PtrLink(const PtrLink <T> &);
	PtrLink <T>&operator=(const PtrLink <T> &);
public:
	T *Value;
	PtrList <T> *List;
	PtrLink <T> *Next;
	PtrLink <T> *Prev;

	PtrLink(PtrList <T> *List,T *Value,PtrLink <T> *Next,PtrLink <T> *Prev) : List(List),Value(Value),Next(Next),Prev(Prev) {}

	PtrLink <T> *Remove()
	{
		PtrLink <T> *Result = Next;
		if (Next)
		{
			Next->Prev = Prev;
		}
		else
		{
			List->Back = Prev;
		}
		if (Prev)
		{
			Prev->Next = Next;
		}
		else
		{
			List->Front = Next;
		}
		delete this;
		return Result;
	}

	~PtrLink()
	{
		delete Value;
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
	PtrLink <T> *Back;

	PtrList() : Front(0) {}

	void Add(T *Value)
	{
		Front = new PtrLink<T>(this,Value,Front,0);
		if (Front->Next)
		{
			Front->Next->Prev = Front;
		}
		else
		{
			Back = Front;
		}
	}

	void Remove(T *Value)
	{
		Find(Value)->Remove();
	}

	PtrLink <T> *Find(T *Value)
	{
		for (PtrLink <T> *it = Front;it;it = it->Next)
		{
			if (it->Value == Value)
			{
				return it;
			}
		}
	}

	void Clear()
	{
		while (Front)
		{
			Front->Remove();
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
		Clear();
	}
};

#endif
