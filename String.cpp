#include "String.h"
#include <assert.h>
#include <cstring>

static size_t strLen(const char * data)
{
	int len = 0;
	for (; data[len] != '\0'; len++);

	return len;
}

String::String()
{
	init();
}

String::String(const size_t size)
{
	this->data = new char[size + 1]{ 0 };
}

String::String(const char * str)
{
	copy(str);
}

String::String(const String & other)
{
	copy(other.data);
}

String & String::operator=(const String & rhs)
{
	if (this != &rhs)
	{
		free();
		copy(rhs.data);
	}
	return *this;
}

const char& String::charAt(int indx) const
{
	return *(data + indx);
}

char& String::charAt(int indx)
{
	return this->data[indx];
}

char * String::toCharArray() const
{
	return data;
}

String String::concat(const char * str)
{
	char* temp = new char[strLen(this->data) + strLen(str) + 1];

	int i = 0;
	for (; i < this->length(); i++)
	{
		temp[i] = this->data[i];
	}
	for (int j = 0; j < strLen(str); j++, i++)
	{
		temp[i] = str[j];
	}
	temp[i] = '\0';

	delete[]this->data;
	this->data = temp;
	mLength = strLen(data);
	return this->data;
}

String String::concat(const String & another)
{
	return concat(another.data);
}

bool String::contains(char s) const
{
	for (int i = 0; i < mLength; ++i)
	{
		if (data[i] == s)
		{
			return true;
		}
	}
	return false;
}

bool String::operator==(const String & other)const
{
	if (this->mLength != other.mLength) return false;

	for (int i = 0; i < mLength; ++i)
	{
		if (this->data[i] != other.data[i])
		{
			return false;
		}
	}
	return true;
}

bool String::operator!=(const String &other) const
{
	return !(*this == other);
}

bool String::operator==(const char * str) const
{
	String strings(str);
	return (*this == strings);
}

bool String::operator!=(const char * str) const
{
	return !(*this == str);
}

bool String::operator<(const String & obj2) const
{
	for (int i = 0; i < this->length(); ++i)
	{
		if (this->data[i] > obj2.data[i])
		{
			return false;
		}
	}
	return true && !(*this == obj2);
}

bool String::operator>(const String & obj2) const
{
	return !(*this < obj2) && !(*this == obj2);
}


bool String::isEmpty() const
{
	return mLength == 0;
}

size_t String::length() const
{
	return mLength;
}

void String::replace(char target, char replacement)
{
	for (int i = 0; i < mLength; ++i)
	{
		if (data[i] == target)
		{
			data[i] = replacement;
		}
	}
}

String String::operator+(const char * str) const
{
	String temp(*this);
	temp.concat(str);
	return temp;
}

String String::operator+(const String & other) const
{
	String temp(*this);
	temp.concat(other);
	return temp;
}


void String::toUpperCase()
{
	for (int i = 0; i < this->length(); ++i)
	{
		if (data[i] >= 'a'&& data[i] < 'A')
		{
			data[i] = data[i] - ('a' - 'A');
		}
	}
}

void String::init()
{
	data = new char[1];
	data[0] = '\0';
	mLength = 0;
}

void String::copy(const char * str)
{
	if (str)
	{
		this->data = new char[strLen(str) + 1];
		strcpy(this->data, str);
		mLength = strLen(str);
		return;
	}
	init();
}

void String::free()
{
	delete[]data;
	data = nullptr;
	mLength = 0;
}

String::~String()
{
	free();
}

std::ostream & operator<<(std::ostream & os, const String & rhs)
{
	os << rhs.data;
	return os;
}

std::istream & operator>>(std::istream & is, String rhs)
{
	is >> rhs.data;
	return is;
}
