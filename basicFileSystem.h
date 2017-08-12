#ifndef BASICFILESYSTEM_H
#define BASICFILESYSTEM_H

#include <iostream>
#include <fstream>

class BasicFileSystem{
private:
	static const int blockSize = 2<<10;
public:
	int getBlockSize();
	virtual int format(const char* name, int size) = 0;
	virtual int read_block(const char* name, int block_index, char *buffer) = 0;
	virtual int write_block(const char* name, int block_index, const char *buffer) = 0;
};


#endif // BASICFILESYSTEM_H