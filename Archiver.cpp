#include "Archiver.h"
#include <fstream>
#include "HuffmanTree.h"
#include <direct.h>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

Archiver::Archiver(const String& sourceDirectory, const String& archive)
	: m_fileTree(sourceDirectory), m_archiveName(archive), m_sourceDirectory(sourceDirectory)
{
}
bool Archiver::compress()
{
	ofstream archive(m_archiveName.toCharArray(), std::ios::binary);
	if (!archive.is_open())
	{
		std::cout << "Cannot create archive " << m_archiveName << std::endl;
		return false;
	}
	// number of directories - 4 bytes
	//  dir_1_len dir_1
	//  dir_2_len dir_2
	//  dir_n_len dir_n
	// file_1 location
	// file_1 size - 4 bytes
	// file_1 data
	const Vector<FileSysEntry>& directories = m_fileTree.getDirectories();
	size_t numberOfDirs = directories.size();
	archive.write((char*)&numberOfDirs, sizeof(numberOfDirs));
	for (size_t i = 0; i < numberOfDirs; i++)
	{
		size_t dirLen = directories.getAt(i).relativePath.length();
		archive.write((char*)&dirLen, sizeof(dirLen));
		archive.write(directories.getAt(i).relativePath.toCharArray(), dirLen);
	}
	//
	const Vector<FileSysEntry>& files = m_fileTree.getFiles();
	size_t numberOfFiles = files.size();
	archive.write((char*)&numberOfFiles, sizeof(numberOfFiles));
	for (size_t i = 0; i < numberOfFiles; i++)
	{
		size_t fileLen = files.getAt(i).relativePath.length();
		archive.write((char*)&fileLen, sizeof(fileLen));
		archive.write(files.getAt(i).relativePath.toCharArray(), fileLen);
		// Compress file
		HuffmanTree compressedFile;
		compressedFile.compress(files.getAt(i).absolutePath);
		// Add compressed file length
		size_t compressedFileLen = compressedFile.getEncodedDataSize();
		archive.write((char*)&compressedFileLen, sizeof(compressedFileLen));
		// Add compressed file data
		archive.write((char*)compressedFile.getEncodedData(), compressedFileLen);
	}
	// Encode file
	archive.close();
	return true;
}

void Archiver::list() const
{
	ifstream archive(m_archiveName.toCharArray(), std::ios::binary);
	if (!archive.is_open())
	{
		std::cout << "Unable to open archive file for decompression\n";
		return;
	}
	size_t numberOfDirectories = 0;
	archive.read((char*)&numberOfDirectories, sizeof(numberOfDirectories));
	Vector<FileSysEntry> directories;
	for (size_t i = 0; i < numberOfDirectories; i++)
	{
		size_t dirLen = 0;
		archive.read((char*)&dirLen, sizeof(dirLen));
		char* dirName = new char[dirLen + 1];
		archive.read(dirName, dirLen);
		dirName[dirLen] = 0;
		FileSysEntry entry;
		entry.entryType = FileSysEntry::Directory;
		entry.relativePath = dirName;
		entry.absolutePath = (m_sourceDirectory + dirName);
		directories.add(entry);
		delete[] dirName;
	}
	size_t numberOfFiles = 0;
	archive.read((char*)&numberOfFiles, sizeof(numberOfFiles));
	Vector<FileSysEntry> files;
	for (size_t i = 0; i < numberOfFiles; i++)
	{
		size_t fileLen = 0;
		archive.read((char*)&fileLen, sizeof(fileLen));
		char* fileName = new char[fileLen + 1];
		archive.read(fileName, fileLen);
		fileName[fileLen] = 0;
		FileSysEntry entry;
		entry.entryType = FileSysEntry::Directory;
		entry.relativePath = fileName;
		entry.absolutePath = (m_sourceDirectory + fileName);
		files.add(entry);
		delete[] fileName;
	}
	archive.close();
	std::cout << "Directories: \n";
	for (size_t i = 0; i < directories.size(); i++)
	{
		cout << i << ". " << directories.getAt(i).absolutePath << endl;
	}
	// Print files
	std::cout << "Files: \n";
	for (size_t i = 0; i < files.size(); i++)
	{
		cout << i << ". " << files.getAt(i).absolutePath << endl;
	}
}
void Archiver::getHelpInfo()
{
	cout << "		---- Welcome to the Archiver Help Center ----\n\n";
	cout << "DESCRIPTION\n\n";
	cout << "	Archiver is one of the best compression utility.\n";
	cout << "	By making files smaller, it saves space on your hard disk and lets you organize files better.\n\n\n\n";
	cout << "ARCHIVES CREATION\n\n";
	cout << "	-compress D:\\My Documents\\DSA C:\\Temp\\myfile.arc\n";
	cout << "		Use the command \"-compress\",";
	cout << " give a valid directory or file path (example: \"D:\\My Documents\\DSA\")\n";
	cout<<"		and specify a file name for your archive (example:\"C:\\Temp\\myfile.arc\").\n\n\n\n";
	cout << "FILES ADDING\n\n";
	cout << "	-add C:\\Test\\test.txt C:\\Temp\\myfile.arc\n";
	cout << "		Use -add, give the path of the new file and the path of an existing archive.\n\n\n\n";
	cout << "CHECKING THE CONTENTS\n\n";
	cout << "	-list C:\\Temp\\myfile.arc\n";
	cout << "		Use -list and the path of an existing archive.\n\n\n\n";
	cout << "FILES EXTRACTING\n\n";
	cout << "	-extract C:\\Temp\\myfile.arc C:\\AnotherDirectory \n";
	cout << "		Use the command \"-extract\", the path of an existing archive and the path,\n";
	cout<<"		where the archive to be extracted.\n\n\n\n";
	cout << "";
}

bool Archiver::decompress()
{
	ifstream archive(m_archiveName.toCharArray(), std::ios::binary);
	if (archive.is_open() == false)
	{
		std::cout << "Can't open archive file for decompression\n";
		return false;
	}
	size_t numberOfDirectories = 0;
	archive.read((char*)&numberOfDirectories, sizeof(numberOfDirectories));
	Vector<FileSysEntry> directories;
	for (size_t i = 0; i < numberOfDirectories; i++)
	{
		size_t dirLen = 0;
		archive.read((char*)&dirLen, sizeof(dirLen));
		char* dirName = new char[dirLen + 1];
		archive.read(dirName, dirLen);
		dirName[dirLen] = 0;
		FileSysEntry entry;
		entry.entryType = FileSysEntry::Directory;
		entry.relativePath = dirName;
		entry.absolutePath = (m_sourceDirectory + dirName);
		directories.add(entry);
		delete[] dirName;
	}
	// Sort directories
	for (size_t i = 0; i < directories.size(); i++)
	{
		for (size_t j = 0; j < directories.size() - i - 1; j++)
		{
			if (directories.getAt(j).relativePath > directories.getAt(j+1).relativePath)
			{
				std::swap(directories.getAt(j), directories.getAt(j + 1));
			}
		}
	}
	// Now create directories
	_mkdir(m_sourceDirectory.toCharArray());
	for (size_t i = 0; i < directories.size(); i++)
	{
		_mkdir(directories.getAt(i).absolutePath.toCharArray());
	}
	//
	size_t numberOfFiles = 0;
	archive.read((char*)&numberOfFiles, sizeof(numberOfFiles));

	unsigned char* buffer = nullptr;
	size_t bufferLen = 0;
	for (size_t i = 0; i < numberOfFiles; i++)
	{
		size_t fileLen = 0;
		archive.read((char*)&fileLen, sizeof(fileLen));
		char* fileName = new char[fileLen + 1];
		archive.read(fileName, fileLen);
		fileName[fileLen] = 0;
		FileSysEntry entry;
		entry.entryType = FileSysEntry::Directory;
		entry.relativePath = fileName;
		entry.absolutePath = (m_sourceDirectory + fileName);
		//
		// Decompress file
		HuffmanTree decompressedFile;
		size_t compressedFileLen = 0;
		archive.read((char*)&compressedFileLen, sizeof(compressedFileLen));
		if (compressedFileLen > bufferLen)
		{
			delete[] buffer;
			bufferLen = compressedFileLen;
			buffer = new unsigned char[bufferLen];
		}
		archive.read((char*)buffer, compressedFileLen);
		decompressedFile.decompress(buffer, compressedFileLen, entry.absolutePath);
		//
		delete[] fileName;
	}
	delete[] buffer;
	archive.close();
	return true;
}

bool Archiver::addToArchive(const String& fileName)
{
	std::fstream archive(m_archiveName.toCharArray(), std::ios::in | std::ios::out | std::ios::binary);
	if (archive.is_open() == false)
	{
		std::cout << "Can't open archive file for decompression\n";
		return false;
	}
	size_t numberOfDirectories = 0;
	archive.read((char*)&numberOfDirectories, sizeof(numberOfDirectories));
	Vector<FileSysEntry> directories;
	for (size_t i = 0; i < numberOfDirectories; i++)
	{
		size_t dirLen = 0;
		archive.read((char*)&dirLen, sizeof(dirLen));
		archive.seekg(dirLen, std::ios::cur);
	}
	archive.seekp(archive.tellg());

	size_t numberOfFiles = 0;
	archive.read((char*)&numberOfFiles, sizeof(numberOfFiles));
	numberOfFiles++;
	archive.write((char*)&numberOfFiles, sizeof(numberOfFiles));
	// Finally go to the end and new file
	archive.seekp(0, std::ios::end);

	size_t idxLastSlash = fileName.length() - 1;
	while (idxLastSlash > 0 && fileName.toCharArray()[idxLastSlash - 1] != '\\')
	{
		idxLastSlash--;
	}
	String onlyFileName;
	onlyFileName.concat(&fileName.toCharArray()[idxLastSlash]);

	String newFileName = (String("\\") + onlyFileName);
	size_t fileLen = newFileName.length();
	archive.write((char*)&fileLen, sizeof(fileLen));
	archive.write(newFileName.toCharArray(), fileLen);
	// Compress file
	HuffmanTree compressedFile;
	compressedFile.compress(fileName);
	// Add compressed file length
	size_t compressedFileLen = compressedFile.getEncodedDataSize();
	archive.write((char*)&compressedFileLen, sizeof(compressedFileLen));
	// Add compressed file data
	archive.write((char*)compressedFile.getEncodedData(), compressedFileLen);

	return true;
}