#ifndef BASICFILESYSTEM_H
#define BASICFILESYSTEM_H

#include <cstdlib>
#include <fstream>
#include <cstring>
#include <iostream>

class BasicFileSystem{
private:
	static const int blockSize = 2<<11;
public:
	int getBlockSize();
	virtual int format(const char* name, unsigned long size) = 0;
	virtual int read_block(const char* name, int block_index, char *buffer) = 0;
	virtual int write_block(const char* name, int block_index, const char *buffer) = 0;
};


#endif // BASICFILESYSTEM_H