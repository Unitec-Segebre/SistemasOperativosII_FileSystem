#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "basicFileSystemDisk.h"
#include "basicFileSystemMemory.h"

class FileSystem{
public:
	enum storage_t{DISK, MEMORY};
	int create_disk(const char* name, storage_t type, unsigned long size);
	int add_file(const char* name, storage_t type, const char* file_name);
	int write_block(const char* name, unsigned long block_index, const char *buffer);
private:
	int getBit(char sequence, int bitNumber);
	int setBit(char* sequence, int bitNumber, char value);
	unsigned long getFreeBlock(const char* name, BasicFileSystem* fs);
};


#endif // FILESYSTEM_H