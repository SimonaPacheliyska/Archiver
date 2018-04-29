#include "Archiver.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3 && argc != 4 && argc != 5)
	{
		cout << "Illegal arguments. Possible arguments are 2, 3, 4 or 5\n";
		return 1;
	}
	if (!strcmp(argv[1], "-compress"))
	{
		Archiver archive(argv[2], argv[3]);
		archive.compress();
	}
	else if (!strcmp(argv[1], "-list"))
	{
		Archiver archive("", argv[2]);
		archive.list();
	}
	else if (!strcmp(argv[1], "-help"))
	{
		Archiver::getHelpInfo();
	}
	else if (!strcmp(argv[1], "-extract"))
	{
		Archiver archive(argv[3], argv[2]);
		archive.decompress();
	}
	else if (strcmp(argv[1], "-add") == 0)
	{
		Archiver archive("", argv[3]);
		archive.addToArchive(argv[2]);
	}
	else
	{
		cout << "Invalid operation. Try again\n";
	}
	system("pause");

	return 0;
}
