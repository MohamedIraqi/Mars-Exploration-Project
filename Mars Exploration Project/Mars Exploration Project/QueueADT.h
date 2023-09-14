#pragma once

#include <string>
using namespace std;

template<typename T>
class QueueADT
{
protected:
	virtual bool isEmpty() const = 0;
	virtual int getCount() const = 0;
	virtual bool enqueue(const T& newEntry) = 0;
	virtual bool enqueue(const T& newEntry, const int& Priority) = 0;
	virtual bool dequeue(T& FrontEntry) = 0;
	virtual bool peek(T& FrontEntry) const = 0;
	virtual string toString() const = 0;
};
