#pragma once
#include <iostream>

template <typename K, typename V>
class Pair {
private:
	K key;
	V value;
public:
	Pair();
	Pair(const K& key, const V& value);
	Pair(const Pair<K, V>&);
	Pair& operator=(const Pair<K, V>&);

	K& getKey();
	const K& getKey()const;

	void setKey(const K&);
	void setValue(const V&);

	bool operator<(const Pair<K, V>&);
	bool operator>(const Pair<K, V>&);
	bool operator==(const Pair<K, V>&);

	V& getValue();
	const V& getValue()const;

	template <typename Key, typename Value>
	friend std::ostream& operator<<(std::ostream& os, const Pair<Key, Value>& p);

private:
	void copy(const Pair<K, V>&);
};

template<typename K, typename V>
Pair<K, V>::Pair() :key(), value()
{
}

template<typename K, typename V>
Pair<K, V>::Pair(const K & key, const V & value)
{
	this->key = key;
	this->value = value;
}

template<typename K, typename V>
Pair<K, V>::Pair(const Pair<K, V>& other)
{
	copy(other);
}

template<typename K, typename V>
Pair<K, V> & Pair<K, V>::operator=(const Pair<K, V>& rhs)
{
	if (this != &rhs)
	{
		copy(rhs);
	}
	return *this;
}

template<typename K, typename V>
K & Pair<K, V>::getKey()
{
	return key;
}

template<typename K, typename V>
const K & Pair<K, V>::getKey()const
{
	return key;
}

template<typename K, typename V>
void Pair<K, V>::setKey(const K & key)
{
	this->key = key;
}

template<typename K, typename V>
void Pair<K, V>::setValue(const V & value)
{
	this->value = value;
}

template<typename K, typename V>
bool Pair<K, V>::operator<(const Pair<K, V>& rhs)
{
	return this->value < rhs.value;
}

template<typename K, typename V>
bool Pair<K, V>::operator>(const Pair<K, V>& rhs)
{
	if (this->value == rhs.value)
	{
		return this->key > rhs.key;
	}
	return !(*this < rhs);
}

template<typename K, typename V>
bool Pair<K, V>::operator==(const Pair<K, V>& rhs)
{
	return (this->key == rhs.key) && (this->value == rhs.value);
}

template<typename K, typename V>
V & Pair<K, V>::getValue()
{
	return value;
}

template<typename K, typename V>
const V & Pair<K, V>::getValue()const
{
	return value;
}

template<typename K, typename V>
void Pair<K, V>::copy(const Pair<K, V>& other)
{
	this->key = other.key;
	this->value = other.value;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Pair<K, V>& p)
{
	os << "<" << p.key << ", " << p.value << ">";
	return os;
}
