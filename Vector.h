#pragma once
#include <assert.h>
#include <iostream>

#define DEFAULT_SIZE 15

template <typename T>
class Vector
{
private:
	T* data;
	size_t mSize;
	size_t capacity;
public:
	Vector();
	Vector(const Vector<T>&);
	Vector& operator=(const Vector<T>&);

	size_t size()const;

	const T& getAt(size_t)const;
	T& getAt(size_t);
	T remove();

	void add(const T& elem);
	void sort();
	bool contains(const T&);
	void print()const;

private:
	void init();
	void clear();
	void copy(const Vector<T>&);
	void resize(size_t);
	void mergeSort(T*, size_t);
	void mergeSortHalves(T*, size_t, T*);
	void merge(T*, size_t, T*, size_t, T*);
};

template<typename T>
Vector<T>::Vector()
{
	init();
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other)
{
	copy(other);
}

template<typename T>
Vector<T> & Vector<T>::operator=(const Vector<T>& rhs)
{
	if (this != &rhs)
	{
		clear();
		init();
		copy(rhs);
	}
	return *this;
}

template<typename T>
size_t Vector<T>::size() const
{
	return mSize;
}

template<typename T>
const T & Vector<T>::getAt(size_t index) const
{
	return data[index];
}

template<typename T>
T & Vector<T>::getAt(size_t index)
{
	return data[index];
}

template<typename T>
T Vector<T>::remove()
{
	assert(mSize);
	T toRm = data[mSize - 1];
	--mSize;
	return toRm;
}

template<typename T>
void Vector<T>::add(const T & elem)
{
	if (mSize + 1 > capacity)
	{
		size_t newSize = (capacity == 0 ? 2 : capacity * 2);
		resize(2 * newSize);
	}
	data[mSize++] = elem;
}

template<typename T>
void Vector<T>::sort()
{
	mergeSort(this->data, mSize);
}

template<typename T>
bool Vector<T>::contains(const T & key)
{
	for (size_t i = 0; i < this->mSize; ++i)
	{
		if (data[i] == key)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
void Vector<T>::print()const
{
	for (size_t i = 0; i < mSize; ++i)
	{
		std::cout << data[i] << " ";
	}
}

template<typename T>
void Vector<T>::init()
{
	mSize = 0;
	capacity = DEFAULT_SIZE;
	data = new T[capacity];
}

template<typename T>
void Vector<T>::clear()
{
	delete[] data;
	data = nullptr;
	mSize = 0;
	capacity = 0;
}

template<typename T>
void Vector<T>::copy(const Vector<T>& other)
{
	data = new T[other.capacity];
	for (size_t i = 0; i < other.mSize; ++i)
	{
		data[i] = other.data[i];
	}
	mSize = other.mSize;
	capacity = other.capacity;
}

template<typename T>
void Vector<T>::resize(size_t newCapacity)
{
	T* temp = new T[newCapacity];
	for (size_t i = 0; i < mSize; ++i)
	{
		temp[i] = data[i];
	}
	delete[] data;
	data = temp;
	capacity = newCapacity;
}

template<typename T>
void Vector<T>::mergeSort(T* vec, size_t mSize)
{
	if (!vec || mSize <= 1)
	{
		return;
	}
	T* result = new T[mSize];
	mergeSortHalves(vec, mSize, result);
}

template<typename T>
void Vector<T>::mergeSortHalves(T* vec, size_t mSize, T* result)
{
	if (mSize <= 1)
	{
		return;
	}
	size_t medium = mSize / 2;
	mergeSortHalves(vec, medium, result);
	mergeSortHalves(vec + medium, mSize - medium, result + medium);
	merge(vec, medium, vec + medium, mSize - medium, result);

	for (size_t i = 0; i < mSize; ++i)
	{
		vec[i] = result[i];
	}
}

template<typename T>
void Vector<T>::merge(T* first, size_t fst, T* second, size_t snd, T* res)
{
	size_t index = 0;
	size_t iFirst = 0;
	size_t iSecond = 0;

	while (iFirst < fst && iSecond < snd)
	{
		if (first[iFirst].getKey() < second[iSecond].getKey())
		{
			res[index++] = first[iFirst++];
			continue;
		}
		else if (first[iFirst].getKey() > second[iSecond].getKey())
		{
			res[index++] = second[iSecond++];
			continue;
		}
		else
		{
			if (first[iFirst].getValue() < second[iSecond].getValue())
			{
				res[index++] = first[iFirst++];
				continue;
			}
			res[index++] = second[iSecond++];
		}
	}

	while (iFirst < fst)
	{
		res[index] = first[iFirst];
		index++;
		iFirst++;
	}
	while (iSecond < snd)
	{
		res[index++] = second[iSecond];
		index++;
		iSecond++;
	}
}

