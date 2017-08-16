//
// Created by segebre on 8/14/17.
//

#ifndef CLION_BASICFILESYSTEMDISK_H
#define CLION_BASICFILESYSTEMDISK_H

#include "BasicFileSystemMemory.h"

class BasicFileSystemDisk : public BasicFileSystem {
public:
    int format(const char* name, unsigned long size);
    int read_block(const char* name, unsigned long block_index, char *buffer);
    int write_block(const char* name, unsigned long block_index, const char *buffer);
};

#endif //CLION_BASICFILESYSTEMDISK_H
