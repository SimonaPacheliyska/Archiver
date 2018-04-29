#include "Cipher.h"
#include <iostream>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

String encryptFile(const String & path, const String & key)
{
	ifstream input(path.toCharArray(), std::ios::binary | std::ios::ate);
	if (!input.is_open()) {
		cout<< "Unable to open a file.\n";
		return "";
	}
	unsigned len = input.tellg();
	input.seekg(0);
	char* data = new char[len + 1]{ 0 };
	input.read(data, len);

	String toEncrypt(data);
	delete[] data;

	for (size_t i = 0; i < toEncrypt.length(); i++) {
		toEncrypt.charAt(i) ^= key.charAt(i % key.length());
	}
	return toEncrypt;
}
