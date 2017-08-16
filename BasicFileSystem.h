//
// Created by segebre on 8/13/17.
//

#ifndef CLION_BASICFILESYSTEM_H
#define CLION_BASICFILESYSTEM_H

class BasicFileSystem{
private:
    static const int blockSize = 2<<11;
public:
    int getBlockSize();
    virtual int format(const char* name, unsigned long size) = 0;
    virtual int read_block(const char* name, unsigned long block_index, char *buffer) = 0;
    virtual int write_block(const char* name, unsigned long block_index, const char *buffer) = 0;
};

#endif //CLION_BASICFILESYSTEM_H
