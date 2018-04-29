#pragma once
#include "HuffmanNode.h"
#include "MinHeap.h"
#include "String.h"

class HuffmanTree
{
public:
	HuffmanTree();
	~HuffmanTree();

	const unsigned char* getEncodedData();
	size_t getEncodedDataSize();

	void compress(const String& fileName);
	void decompress( const unsigned char* compressedData, 
					 size_t compressedDataSize, 
					 const String& destination);

private:
	void buildHuffmanTree(MinHeap<Pair<char, size_t>>);
	void generateCodes(const HuffmanNode*, Vector<Pair<char, String>>&, const String&);
	void countsOccrncs(const String&, MinHeap<Pair<char, size_t>>&);
	void eraseTree(HuffmanNode*& root);

private:
	HuffmanNode* root;
	unsigned char* data;
	size_t size;
};
