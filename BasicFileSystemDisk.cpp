//
// Created by segebre on 8/14/17.
//

#include "BasicFileSystemDisk.h"
#include <cstdlib>
#include <fstream>

using namespace std;

int BasicFileSystemDisk::format(const char* name, unsigned long size){
    ofstream outfile;
    outfile.open(name, ios::binary | ios::out | ios::trunc);
    if(!outfile)
        return -1;
    outfile.seekp(size-1, ios::beg);
    outfile.write("", 1);
    outfile.close();
}

int BasicFileSystemDisk::read_block(const char* name, unsigned long block_index, char *buffer){
    ifstream infile;
    infile.open(name, ios::binary | ios::in);
    if(!infile)
        return -1;
    infile.seekg(block_index*getBlockSize(), ios::beg);
    infile.read(buffer, getBlockSize());
    infile.close();
}

int BasicFileSystemDisk::write_block(const char* name, unsigned long block_index, const char *buffer){
    ofstream outfile;
    outfile.open(name, ios::binary | ios::out | ios::in);
    if(!outfile)
        return -1;
    outfile.seekp(block_index*getBlockSize(), ios::beg);
    outfile.write(buffer, getBlockSize());
    outfile.close();
}
