//
// Created by segebre on 8/13/17.
//

#ifndef CLION_BASICFILESYSTEMMEMORY_H
#define CLION_BASICFILESYSTEMMEMORY_H

#include "BasicFileSystem.h"

class BasicFileSystemMemory : public BasicFileSystem {
public:
    int format(const char* name, unsigned long size);
    int read_block(const char* name, unsigned long block_index, char *buffer);
    int write_block(const char* name, unsigned long block_index, const char *buffer);
};

#endif //CLION_BASICFILESYSTEMMEMORY_H
