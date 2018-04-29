#include "FileSysTree.h"
#include "dirent.h"

FileSysTree::FileSysTree(const String & rootDir_)
	: rootDir(rootDir_)
{
	buildHelper(rootDir, "", directories, files);
}

const String& FileSysTree::getRoot() const
{
	return rootDir;
}

const Vector<FileSysEntry>& FileSysTree::getDirectories() const
{
	return directories;
}

const Vector<FileSysEntry>& FileSysTree::getFiles() const
{
	return files;
}

void FileSysTree::buildHelper(const String& absolute,
	const String& relative,
	Vector<FileSysEntry>& directories_,
	Vector<FileSysEntry>& files_)
{
	DIR *dir;
	struct dirent* ent;

	if (dir = opendir(absolute.toCharArray()))
	{
		while (ent = readdir(dir))
		{
			if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
			{
				continue;
			}

			String path(absolute);
			path = path + "\\" + ent->d_name;
			String relPath(relative);
			relPath = relPath + "\\" + ent->d_name;

			FileSysEntry entry;
			entry.absolutePath = path;
			entry.relativePath = relPath;

			if (ent->d_type == DT_DIR)
			{
				buildHelper(path, relPath, directories_, files_);
				entry.entryType = FileSysEntry::Directory;
				directories_.add(entry);
			}
			else
			{
				entry.entryType = FileSysEntry::File;
				files_.add(entry);
			}
		}
		closedir(dir);
	}
}
