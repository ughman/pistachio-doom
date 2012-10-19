#ifndef Queue_hpp
#define Queue_hpp

#include "List.hpp"
#include "Exception.hpp"

template <typename T>
class Queue
{
private:
	Queue(const Queue <T> &);
	Queue <T>&operator=(const Queue <T> &);
public:
	class EmptyException : public Exception
	{
	public:
		virtual const char *Message() const { return "Dequeue on empty queue."; }

		virtual ~EmptyException() {}
	};

	List <T> Elements;

	Queue <T>()
	{
	}

	void Enqueue(T Value)
	{
		Elements.Add(Value);
	}

	T Dequeue()
	{
		if (Elements.Front)
		{
			T Result = Elements.Front->Value;
			Elements.Front->Remove();
			return Result;
		}
		else
		{
			throw EmptyException();
		}
	}

	void Clear()
	{
		Elements.Clear();
	}
};

#endif
