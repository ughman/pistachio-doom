#ifndef List_hpp
#define List_hpp

template <typename T>
class List;

template <typename T>
class Link
{
private:
	Link(const Link <T> &);
	Link <T>&operator=(const Link <T> &);
public:
	T Value;
	List <T> *Owner;
	Link <T> *Next;
	Link <T> *Prev;

	Link(List <T> *Owner,T Value,Link <T> *Next,Link <T> *Prev) : Owner(Owner),Value(Value),Next(Next),Prev(Prev) {}

	Link <T> *Remove()
	{
		Link <T> *Result = Next;
		if (Next)
		{
			Next->Prev = Prev;
		}
		else
		{
			Owner->Back = Prev;
		}
		if (Prev)
		{
			Prev->Next = Next;
		}
		else
		{
			Owner->Front = Next;
		}
		delete this;
		return Result;
	}
};

template <typename T>
class List
{
private:
	List <T>(const List <T> &);
	List <T>&operator=(const List <T> &);
public:
	Link <T> *Front;
	Link <T> *Back;

	List() : Front(0) {}

	void Add(T Value)
	{
		Front = new Link<T>(this,Value,Front,0);
		if (Front->Next)
		{
			Front->Next->Prev = Front;
		}
		else
		{
			Back = Front;
		}
	}

	void Remove(T Value)
	{
		Find(Value)->Remove();
	}

	Link <T> *Find(T Value)
	{
		for (Link <T> *it = Front;it;it = it->Next)
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
		for (Link <T> *it = Front;it;it = it->Next)
		{
			Result++;
		}
		return Result;
	}

	~List()
	{
		Clear();
	}
};

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
	PtrList <T> *Owner;
	PtrLink <T> *Next;
	PtrLink <T> *Prev;

	PtrLink(PtrList <T> *Owner,T *Value,PtrLink <T> *Next,PtrLink <T> *Prev) : Owner(Owner),Value(Value),Next(Next),Prev(Prev) {}

	PtrLink <T> *Remove()
	{
		PtrLink <T> *Result = Next;
		if (Next)
		{
			Next->Prev = Prev;
		}
		else
		{
			Owner->Back = Prev;
		}
		if (Prev)
		{
			Prev->Next = Next;
		}
		else
		{
			Owner->Front = Next;
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
