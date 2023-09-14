#pragma once

#include "Node.h"
#include "QueueADT.h"
#include "Mission.h"
#include "Rover.h"
#include "Event.h"
#include <string>
using namespace std;

template <typename T>
class LinkedQueue :public QueueADT<T>
{
protected:
	Node<T>* frontPtr;
	Node<T>* backPtr;
	int count;
	virtual bool enqueue(const T& newEntry, const int& Priority);
public:
	LinkedQueue();
	LinkedQueue(const LinkedQueue<T>& LQ);
	bool isEmpty() const;
	int getCount() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry) const;
	string toString() const;
	~LinkedQueue();
};

template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	frontPtr = nullptr;
	backPtr = nullptr;
	count = 0;
}

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& LQ)
{
	Node<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr)
	{
		frontPtr = nullptr;
		backPtr = nullptr;
		count = 0;
		return;
	}

	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	frontPtr = ptr;
	backPtr = ptr;
	++count;
	NodePtr = NodePtr->getNext();

	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		++count;
		NodePtr = NodePtr->getNext();
	}
}

template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);

	if (isEmpty())
		frontPtr = newNodePtr;
	else
		backPtr->setNext(newNodePtr);

	backPtr = newNodePtr;
	++count;
	return true;
}

template<typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry, const int& Priority)
{
	return false;
}

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();

	if (nodeToDeletePtr == backPtr)
		backPtr = nullptr;

	delete nodeToDeletePtr;
	--count;
	return true;

}

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}

template<typename T>
int LinkedQueue<T>::getCount() const
{
	return count;
}

template <typename T>
string LinkedQueue<T>::toString() const
{
	string S;

	Node<T>* Trv = frontPtr;

	while (Trv)
	{
		S.append(to_string(Trv->getItem()));
		S.append(" ");

		Trv = Trv->getNext();
	}

	return S;
}

template <>
inline string LinkedQueue<Event*>::toString() const
{
	string F;

	Node<Event*>* Trv = frontPtr;
	Event* Ev;

	while (Trv)
	{
		Ev = Trv->getItem();
		if (Ev->getEventType() == EventTypes::Formulation_Event)
		{
			if (F.empty())
			{
				F.append("|");
				F.append("F");
				F.append("/");
				F.append(to_string(Ev->getEventDay()));
			}
			else
			{
				F.append(", ");
				F.append("F");
				F.append("/");
				F.append(to_string(Ev->getEventDay()));
			}
		}
		Trv = Trv->getNext();
	}

	if (!F.empty())
	{
		F.append("| ");
	}

	return F;
}

template <>
inline string LinkedQueue<Mission*>::toString() const
{
	string E;
	string P;

	Node<Mission*>* Trv = frontPtr;
	Mission* Mis;

	while (Trv)
	{
		Mis = Trv->getItem();

		if (!(Mis->getFailureDay() < Mis->getCompletionDay() && !Mis->getAssignedRover()))
		{
			if (Mis->getMissionType() == MissionTypes::Emergency_Mission)
			{
				if (E.empty())
				{
					E.append("[");
					E.append(to_string(Mis->getID()));
				}
				else
				{
					E.append(", ");
					E.append(to_string(Mis->getID()));
				}

				if (Mis->getAssignedRover())
				{
					E.append("/");
					E.append(to_string(Mis->getAssignedRover()->getID()));
				}
			}
			else if (Mis->getMissionType() == MissionTypes::Polar_Mission)
			{
				if (P.empty())
				{
					P.append("(");
					P.append(to_string(Mis->getID()));
				}
				else
				{
					P.append(", ");
					P.append(to_string(Mis->getID()));
				}

				if (Mis->getAssignedRover())
				{
					P.append("/");
					P.append(to_string(Mis->getAssignedRover()->getID()));
				}
			}
		}

		Trv = Trv->getNext();
	}

	if (!E.empty())
	{
		E.append("] ");
	}
	if (!P.empty())
	{
		P.append(") ");
	}

	return E + P;
}

template <>
inline string LinkedQueue<Rover*>::toString() const
{
	string E;
	string P;

	Node<Rover*>* Trv = frontPtr;
	Rover* Rov;

	while (Trv)
	{
		Rov = Trv->getItem();
		if (Rov->getRoverType() == RoverTypes::Emergency_Rover)
		{
			if (E.empty())
			{
				E.append("[");
				E.append(to_string(Rov->getID()));
			}
			else
			{
				E.append(", ");
				E.append(to_string(Rov->getID()));
			}
		}
		else if (Rov->getRoverType() == RoverTypes::Polar_Rover)
		{
			if (P.empty())
			{
				P.append("(");
				P.append(to_string(Rov->getID()));
			}
			else
			{
				P.append(", ");
				P.append(to_string(Rov->getID()));
			}
		}
		Trv = Trv->getNext();
	}

	if (!E.empty())
	{
		E.append("] ");
	}
	if (!P.empty())
	{
		P.append(") ");
	}

	return  E + P;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	T temp;
	while (dequeue(temp));

	count = 0;
}