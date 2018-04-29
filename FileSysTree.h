#pragma once
#include "Vector.h"
#include "String.h"

struct FileSysEntry
{
	enum FileSysEntryType
	{
		File,
		Directory
	};

	FileSysEntryType entryType;
	String relativePath; // relative to root
	String absolutePath;
};

class FileSysTree
{
private:
	Vector<FileSysEntry> directories;
	Vector<FileSysEntry> files;
	String rootDir;
public:
	FileSysTree(const String& root);

	const String& getRoot() const;
	const Vector<FileSysEntry>& getDirectories() const;
	const Vector<FileSysEntry>& getFiles() const;

private:
	void buildHelper(const String& absolute,
		const String& relative,
		Vector<FileSysEntry>& direcotires,
		Vector<FileSysEntry>& files);
};