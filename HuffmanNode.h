#pragma once
#include "String.h"

struct HuffmanNode
{
	char code;
	unsigned long occurences;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(char code_ = '\0', unsigned long occurences_ = 0,
		HuffmanNode* left_ = nullptr, HuffmanNode* right_ = nullptr)
		: code(code_), occurences(occurences_), left(left_), right(right_)
	{
	}

	HuffmanNode(const HuffmanNode& rhs)
		: code(rhs.code), occurences(rhs.occurences), left(rhs.left), right(rhs.right)
	{
	}

	HuffmanNode& operator=(const HuffmanNode& rhs)
	{
		if (this != &rhs)
		{
			code = rhs.code;
			occurences = rhs.occurences;
			left = rhs.left;
			right = rhs.right;

		}
		return *this;
	}

	bool operator > (const HuffmanNode& rhs) const
	{
		return occurences > rhs.occurences;
	}
};
