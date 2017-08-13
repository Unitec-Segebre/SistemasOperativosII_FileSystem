#ifndef BASICFILESYSTEMDISK_H
#define BASICFILESYSTEMDISK_H

#include "basicFileSystem.h"

class BasicFileSystemDisk : public BasicFileSystem {
public:
	int format(const char* name, unsigned long size);
	int read_block(const char* name, unsigned long block_index, char *buffer);
	int write_block(const char* name, unsigned long block_index, const char *buffer);
};

#endif // BASICFILESYSTEMDISK_H