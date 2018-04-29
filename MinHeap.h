#pragma once
#include "Pair.h"
#include "Vector.h"


template <typename T>
class MinHeap
{
private:
	Vector<T> heap;
public:
	MinHeap();
	MinHeap(const MinHeap<T>&);
	MinHeap& operator=(const MinHeap<T>&);
	~MinHeap();

	void print()const;
	void insert(const T&);
	T getMin();
	size_t size()const;

private:
	void init();
	void copy(const MinHeap<T>&);
	void heapifyUp(const size_t&);
	void heapifyDown(const size_t&);
	void swap(T&, T&);
};

template <typename T>
void MinHeap<T>::print() const
{
	heap.print();
}

template <typename T>
void MinHeap<T>::insert(const T & elem)
{
	heap.add(elem);
	heapifyUp(heap.size() - 1);
}

template <typename T>
T MinHeap<T>::getMin()
{
	if (heap.size() == 0)
	{
		assert(heap.size());
	}

	T elemToRm = heap.getAt(0);
	swap(heap.getAt(0), heap.getAt(heap.size() - 1));
	heap.remove();
	this->heapifyDown(0);

	return elemToRm;
}

template<typename T>
size_t MinHeap<T>::size() const
{
	return heap.size();
}

template <typename T>
MinHeap<T>::MinHeap()
{
	init();
}

template <typename T>
MinHeap<T>::MinHeap(const MinHeap& other)
{
	init();
	copy(other);
}

template <typename T>
MinHeap<T> & MinHeap<T>::operator=(const MinHeap<T>& rhs)
{
	if (this != &rhs)
	{
		init();
		copy(rhs);
	}
	return *this;
}

template <typename T>
MinHeap<T>::~MinHeap()
{
}

template <typename T>
void MinHeap<T>::init()
{
	heap = Vector<T>();
}

template <typename T>
void MinHeap<T>::copy(const MinHeap<T>& other)
{
	this->heap = other.heap;
}

template <typename T>
void MinHeap<T>::heapifyUp(const size_t & indx)
{
	if (!indx)
	{
		return;
	}
	size_t parent = (indx - 1) / 2;
	if (this->heap.getAt(parent) > this->heap.getAt(indx))
	{
		swap(this->heap.getAt(indx), this->heap.getAt(parent));
		this->heapifyUp(parent);
	}
}

template <typename T>
void MinHeap<T>::heapifyDown(const size_t & indx)
{

	size_t leftIndx = 2 * indx + 1;
	size_t rightIndx = 2 * indx + 2;

	if (leftIndx >= heap.size())
	{
		return;
	}
	size_t currMin = indx;

	if (heap.getAt(currMin) > heap.getAt(leftIndx))
	{
		currMin = leftIndx;
	}
	if (heap.getAt(currMin) > heap.getAt(rightIndx) && rightIndx < heap.size())
	{
		currMin = rightIndx;
	}
	if (currMin != indx)
	{
		swap(heap.getAt(currMin), heap.getAt(indx));
		heapifyDown(currMin);
	}
}

template <typename T>
void MinHeap<T>::swap(T& fst, T& snd)
{
	T temp = fst;
	fst = snd;
	snd = temp;
}

