#pragma once

template <typename T>
class Node
{
private:
	const T item;
	int priority;
	Node<T>* next;
public:
	Node(const T& r_Item, Node<T>* nextNodePtr = nullptr);
	Node(const T& r_Item, int r_priority, Node<T>* nextNodePtr = nullptr);

	T getItem() const;

	void setPriority(int r_Priority);
	int getPriority() const;

	void setNext(Node<T>* nextNodePtr);
	Node<T>* getNext() const;
};

template <typename T>
Node<T>::Node(const T& r_Item, Node<T>* nextNodePtr)
	: item(r_Item)
{
	priority = 0;
	next = nextNodePtr;
}

template<typename T>
Node<T>::Node(const T& r_Item, int r_Priority, Node<T>* nextNodePtr)
	: item(r_Item)
{
	priority = r_Priority;
	next = nextNodePtr;
}

template <typename T>
T Node<T>::getItem() const
{
	return item;
}

template<typename T>
void Node<T>::setPriority(int r_Priority)
{
	priority = r_Priority;
}

template<typename T>
int Node<T>::getPriority() const
{
	return priority;
}

template <typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template <typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
}