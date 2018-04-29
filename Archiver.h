#pragma once
#include "FileSysTree.h"
#include "HuffmanTree.h"

#include "String.h"
class Archiver
{
public:
	Archiver(const String& sourceDirectory, const String& archive);
	bool compress();
	void list()const;
	static void getHelpInfo();
	bool decompress();
	bool addToArchive(const String& fileName);
private:
	FileSysTree m_fileTree;
	String m_archiveName;
	String m_sourceDirectory;
};