#ifndef HashTable_hpp
#define HashTable_hpp

#include "Exception.hpp"

#define HASH_RANGE 256

template <typename K,typename V>
class HashTable;

template <typename K,typename V>
class HashLink
{
private:
	HashLink(const HashLink <K,V> &);
	HashLink <K,V> &operator=(const HashLink <K,V> &);
public:
	K Key;
	V Value;
	HashTable <K,V> *Owner;
	unsigned int Hash;
	HashLink <K,V> *Next;
	HashLink <K,V> *Prev;

	HashLink(HashTable <K,V> *Owner,K Key,V Value,unsigned int Hash,HashLink <K,V> *Next,HashLink <K,V> *Prev) : Owner(Owner),Key(Key),Value(Value),Hash(Hash),Next(Next),Prev(Prev) {}

	HashLink <K,V> *Remove()
	{
		HashLink <K,V> *Result = Next;
		if (Next)
		{
			Next->Prev = Prev;
		}
		else
		{
			Owner->Back[Hash] = Prev;
		}
		if (Prev)
		{
			Prev->Next = Next;
		}
		else
		{
			Owner->Front[Hash] = Next;
		}
		delete this;
		return Result;
	}
};

template <typename K,typename V>
class HashTable
{
private:
	HashTable(const HashTable <K,V> &);
	HashTable <K,V> &operator=(const HashTable <K,V> &);
public:
	class KeyNotFoundException : public Exception
	{
	public:
		virtual const char *Message() const { return "Key not found in hashtable."; }

		virtual ~KeyNotFoundException() {}
	};

	class DuplicateKeyException : public Exception
	{
	public:
		virtual const char *Message() const { return "Key already exists in hashtable."; }

		virtual ~DuplicateKeyException() {}
	};

	HashLink <K,V> *Front[HASH_RANGE];
	HashLink <K,V> *Back[HASH_RANGE];

	HashTable()
	{
		for (unsigned int i = 0;i < HASH_RANGE;i++)
		{
			Front[i] = 0;
			Back[i] = 0;
		}
	}

	void Add(K Key,V Value)
	{
		unsigned int Hash = Key.Hash() % HASH_RANGE;
		for (HashLink <K,V> *it = Front[Hash];it;it = it->Next)
		{
			if (it->Key == Key)
			{
				throw DuplicateKeyException();
			}
		}
		Back[Hash] = new HashLink<K,V>(this,Key,Value,Hash,0,Back[Hash]);
		if (Back[Hash]->Prev)
		{
			Back[Hash]->Prev->Next = Back[Hash];
		}
		else
		{
			Front[Hash] = Back[Hash];
		}
	}

	void Remove(K Key)
	{
		Find(Key)->Remove();
	}

	HashLink <K,V> *Find(K Key)
	{
		unsigned int Hash = Key.Hash() % HASH_RANGE;
		for (HashLink <K,V> *it = Front[Hash];it;it = it->Next)
		{
			if (it->Key == Key)
			{
				return it;
			}
		}
		throw KeyNotFoundException();
	}

	void Clear()
	{
		for (unsigned int i = 0;i < HASH_RANGE;i++)
		{
			while (Front[i])
			{
				Front[i]->Remove();
			}
		}
	}

	~HashTable()
	{
		Clear();
	}
};

#endif
