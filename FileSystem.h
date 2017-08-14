#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "basicFileSystemDisk.h"
#include "basicFileSystemMemory.h"

class FileSystem{
private:
	int getBit(char sequence, int bitNumber);
	int setBit(char* sequence, int bitNumber, char value);
public:
	enum storage_t{DISK, MEMORY};
	int format(const char* name, unsigned long size, storage_t type);
	int read_block(const char* name, unsigned long block_index, char *buffer);
	int write_block(const char* name, unsigned long block_index, const char *buffer);
};


#endif // FILESYSTEM_H