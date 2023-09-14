#pragma once

#include "LinkedQueue.h"

template<typename T>
class LinkedPriorityQueue : public LinkedQueue<T>
{
public:
	LinkedPriorityQueue();
	LinkedPriorityQueue(const LinkedPriorityQueue<T>& LQ);
	bool enqueue(const T& newEntry, const int& Priority);
	~LinkedPriorityQueue();
};


template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue()
{
	this->frontPtr = nullptr;
	this->backPtr = nullptr;
	this->count = 0;
}

template <typename T>
LinkedPriorityQueue<T>::LinkedPriorityQueue(const LinkedPriorityQueue<T>& LPQ)
{
	Node<T>* NodePtr = LPQ.frontPtr;
	if (!NodePtr)
	{
		this->frontPtr = nullptr;
		this->backPtr = nullptr;
		count = 0;
		return;
	}

	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	this->frontPtr = ptr;
	this->backPtr = ptr;
	++(this->count);
	NodePtr = NodePtr->getNext();

	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		this->backPtr->setNext(ptr);
		this->backPtr = ptr;
		++(this->count);
		NodePtr = NodePtr->getNext();
	}
}

template <typename T>
bool LinkedPriorityQueue<T>::enqueue(const T& newEntry, const int& Priority)
{
	Node<T>* Trv = this->frontPtr;
	Node<T>* newNodePtr = new Node<T>(newEntry, Priority);

	if (this->isEmpty())
	{
		this->frontPtr = newNodePtr;
		this->backPtr = newNodePtr;
		++(this->count);
		return true;
	}
	else
	{
		if (Trv->getPriority() < newNodePtr->getPriority())
		{
			newNodePtr->setNext(Trv);
			this->frontPtr = newNodePtr;
			++(this->count);
			return true;
		}

		while (Trv->getNext() && Trv->getNext()->getPriority() >= newNodePtr->getPriority())
		{
			Trv = Trv->getNext();

			if (Trv->getNext() == nullptr)
			{
				newNodePtr->setNext(nullptr);
				Trv->setNext(newNodePtr);
				this->backPtr = newNodePtr;
				++(this->count);
				return true;
			}
		}

		newNodePtr->setNext(Trv->getNext());
		Trv->setNext(newNodePtr);
		++(this->count);
		return true;
	}
}

template <typename T>
LinkedPriorityQueue<T>::~LinkedPriorityQueue()
{
	T temp;
	while (this->dequeue(temp));

	this->count = 0;
}