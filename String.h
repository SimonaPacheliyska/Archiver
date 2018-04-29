#pragma once
#include <istream>

class String
{
public:
	String();
	String(const size_t);
	String(const char* str);
	String(const String& other);
	String& operator=(const String& rhs);
	~String();


	const char& charAt(int indx)const;
	char& charAt(int indx);

	char* toCharArray()const;

	String concat(const char*);
	String concat(const String& another);
	bool contains(char s) const;

	bool operator==(const String& other)const;
	bool operator!=(const String&)const;
	bool operator==(const char* str)const;
	bool operator!=(const char* str)const;
	bool operator<(const String& obj2)const;
	bool operator>(const String& obj2)const;

	bool isEmpty()const;
	size_t length()const;

	void replace(char target, char replacement);
	String operator+(const char*) const;
	String operator+(const String& other) const;

	friend std::ostream& operator<<(std::ostream& os, const String& rhs);
	friend std::istream& operator>>(std::istream& is, String rhs);

	void toUpperCase();


private:
	void init();
	void copy(const char*);
	void free();

private:
	char* data;
	size_t mLength;
};

