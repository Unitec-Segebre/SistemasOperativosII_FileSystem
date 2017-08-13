#ifndef BASICFILESYSTEMMEMORY_H
#define BASICFILESYSTEMMEMORY_H

#include "basicFileSystem.h"

class BasicFileSystemMemory : public BasicFileSystem {
public:
	int format(const char* name, unsigned long size);
	int read_block(const char* name, unsigned long block_index, char *buffer);
	int write_block(const char* name, unsigned long block_index, const char *buffer);
};

#endif // BASICFILESYSTEMMEMORY_H