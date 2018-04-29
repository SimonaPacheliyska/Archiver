#include "HuffmanTree.h"
#include "Archiver.h"
#include <fstream>
#include <string.h>

HuffmanTree::HuffmanTree()
	:root(nullptr), data(nullptr), size(0) {}

HuffmanTree::~HuffmanTree()
{
	eraseTree(root);
}

const unsigned char* HuffmanTree::getEncodedData()
{
	return data;
}

size_t HuffmanTree::getEncodedDataSize()
{
	return size;
}

void HuffmanTree::buildHuffmanTree(MinHeap<Pair<char, size_t>> minHeap)
{
	if (root != nullptr)
	{
		eraseTree(root);
	}
	MinHeap<HuffmanNode> helperHeap;
	while (minHeap.size() > 0)
	{
		Pair<char, size_t> element = minHeap.getMin();
		HuffmanNode nodeToAdd(element.getKey(), element.getValue());
		helperHeap.insert(nodeToAdd);
	}

	while (helperHeap.size() > 1)
	{
		HuffmanNode* left = new HuffmanNode(helperHeap.getMin());
		HuffmanNode* right = new HuffmanNode(helperHeap.getMin());

		HuffmanNode merged;
		merged.left = left;
		merged.right = right;
		helperHeap.insert(merged);
	}

	root = new HuffmanNode(helperHeap.getMin());
}

void HuffmanTree::generateCodes(const HuffmanNode * root, Vector<Pair<char, String>>& entry, const String& code)
{
	if (!root->left && !root->right)
	{
		Pair<char, String> pair(root->code, code);
		if (!entry.contains(pair))
		{
			entry.add(pair);
		}
	}
	//
	if (root->left)
	{
		generateCodes(root->left, entry, code + "0");
	}
	//
	if (root->right)
	{
		generateCodes(root->right, entry, code + "1");
	}
}

void HuffmanTree::countsOccrncs(const String& fileName, MinHeap<Pair<char, size_t>>& minHeap)
{
	std::ifstream read(fileName.toCharArray(), std::ios::binary);
	if (!read.is_open())
	{
		return;
	}
	//
	const int maxElements = 256;
	size_t occurences[maxElements] = { 0 };
	while (true)
	{
		unsigned char byte;
		read.get((char&)byte);

		if (read.eof())
		{
			break;
		}
		occurences[byte]++;
	}
	//
	for (int i = 0; i < maxElements; i++)
	{
		if (occurences[i] != 0)
		{
			minHeap.insert(Pair<char, size_t>(char(i), occurences[i]));
		}
	}
}

void HuffmanTree::eraseTree(HuffmanNode*&root)
{
	if (root)
	{
		eraseTree(root->left);
		eraseTree(root->right);
		delete root;
		root = nullptr;
	}
}

void HuffmanTree::compress(const String& fileName)
{
	MinHeap<Pair<char, size_t>> minHeap;
	countsOccrncs(fileName, minHeap);

	buildHuffmanTree(minHeap);

	Vector<Pair<char, String>> v;
	generateCodes(this->root, v, "0");

	std::ifstream read(fileName.toCharArray(), std::ios::binary);
	if (!read.is_open())
	{
		return;
	}
	//
	unsigned char writer = 0;
	int usedBits = 0;
	int maxBits = sizeof(writer) * 8;
	int currentBitIdx = 0;
	//
	unsigned int symbolsToWrite = 0;
	while (true)
	{
		char byte;
		read.get(byte);

		if (read.eof())
		{
			break;
		}

		for (size_t i = 0; i < v.size(); i++)
		{
			if (byte == v.getAt(i).getKey())
			{
				symbolsToWrite += v.getAt(i).getValue().length();
			}
		}
	}
	read.clear();
	read.seekg(0);
	if (data != nullptr)
	{
		delete[] data;
	}
	size = (symbolsToWrite / 8) + 1;
	// Add for code table
	size_t sizeCodeTable = (minHeap.size() * (1 + sizeof(size_t)));
	size += 4; // 4 bytes for code table size
	size += sizeCodeTable;
	size += 4; // 4 bytes for number of symbols to read later
			   // Memory for compressed data
	data = new unsigned char[size];
	size_t currentIdx = 0;
	//
	size_t tableSize = minHeap.size();
	memcpy(&data[currentIdx], &tableSize, sizeof(tableSize));
	currentIdx += 4;
	// Add compressed table
	while (minHeap.size() > 0)
	{
		Pair<char, size_t> el = minHeap.getMin();
		memcpy(&data[currentIdx], &el.getKey(), sizeof(el.getKey()));
		currentIdx++;
		memcpy(&data[currentIdx], &el.getValue(), sizeof(el.getValue()));
		currentIdx += sizeof(size_t);
	}
	//
	memcpy(&data[currentIdx], &symbolsToWrite, sizeof(symbolsToWrite));
	currentIdx += 4;
	//
	while (true)
	{
		char byte;
		read.get(byte);

		if (read.eof())
		{
			break;
		}

		for (size_t i = 0; i < v.size(); i++)
		{
			if (byte == v.getAt(i).getKey())
			{
				String& code = v.getAt(i).getValue();
				bool finishedWriting = false;
				int remainingBits = code.length();
				int currentCodePos = 0;
				while (!finishedWriting)
				{
					if (remainingBits >(maxBits - currentBitIdx))
					{
						// Write only maxBits - usedBits
						int bitsToWrite = (maxBits - currentBitIdx);
						remainingBits -= bitsToWrite;
						//
						for (int i = 0; i < bitsToWrite; i++)
						{
							if (code.charAt(currentCodePos) == '1')
							{
								writer |= (1ull << (7 - currentBitIdx));
							}
							currentBitIdx++;
							currentCodePos++;
						}
						//
						data[currentIdx++] = writer;
						currentBitIdx = 0;
						writer = 0;
					}
					else
					{
						for (int i = 0; i < remainingBits; i++)
						{
							if (code.charAt(currentCodePos) == '1')
							{
								writer |= (1ull << (7 - currentBitIdx));
							}
							currentBitIdx++;
							currentCodePos++;
						}
						finishedWriting = true;
					}
				}
				break;
			}
		}
	}
	//
	if (currentBitIdx != 0)
	{
		// flush last part of the file
		data[currentIdx++] = writer;
	}
}

void HuffmanTree::decompress( const unsigned char* compressedData,
							  size_t compressedDataSize,
							  const String& destination)
{
	std::ofstream write(destination.toCharArray());

	if (compressedData == nullptr)
	{
		return;
	}

	size_t currentByteIdx = 0;
	unsigned int codeTableSize = 0;

	memcpy(&codeTableSize, &compressedData[currentByteIdx], sizeof(codeTableSize));
	currentByteIdx += sizeof(codeTableSize);

	MinHeap<Pair<char, size_t>> minHeap;
	for (unsigned int i = 0; i < codeTableSize; i++)
	{
		char code = 0;
		size_t occurences = 0;

		memcpy(&code, &compressedData[currentByteIdx], sizeof(code));
		currentByteIdx += sizeof(code);

		memcpy(&occurences, &compressedData[currentByteIdx], sizeof(occurences));
		currentByteIdx += sizeof(occurences);

		minHeap.insert(Pair<char, size_t>(code, occurences));
	}

	buildHuffmanTree(minHeap);
	Vector<Pair<char, String>> v;
	generateCodes(this->root, v, "0");

	unsigned int symbolsToRead = 0;
	memcpy(&symbolsToRead, &compressedData[currentByteIdx], sizeof(symbolsToRead));
	currentByteIdx += sizeof(symbolsToRead);
	//
	bool shouldStop = false;
	unsigned int symbols = 0;
	//String decompressed;
	char* decompressed = new char[symbolsToRead];
	while (true && shouldStop == false)
	{
		unsigned char byte;

		memcpy(&byte, &compressedData[currentByteIdx], sizeof(byte));
		currentByteIdx += sizeof(byte);

		if (currentByteIdx > compressedDataSize)
		{
			break;
		}

		for (int i = 0; i < sizeof(byte) * 8; i++)
		{
			if (byte & (1 << (7 - i)))
			{
				decompressed[symbols] = '1';
			}
			else
			{
				decompressed[symbols] = '0';
			}
			symbols++;
			if (symbols >= symbolsToRead)
			{
				shouldStop = true;
				break;
			}
		}
	}
	//
	// Decompress file using code table
	size_t currentIdx = 1;
	HuffmanNode* currNode = root;
	while (true)
	{
		if (!currNode->left && !currNode->right)
		{
			char byteToWrite = currNode->code;
			write.write(&byteToWrite, sizeof(byteToWrite));
			currNode = root;
		}
		else if (decompressed[currentIdx] == '0')
		{
			currNode = currNode->left;
		}
		else if (decompressed[currentIdx] == '1')
		{
			currNode = currNode->right;
		}
		currentIdx++;

		if (currentIdx > symbolsToRead)
		{
			break;// No more data to decompress
		}
	}
	write.close();
	delete[] decompressed;
}
