//
// Created by segebre on 8/13/17.
//

#include "BasicFileSystemMemory.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;

int BasicFileSystemMemory::format(const char* name, unsigned long size){
    int shm_fd = shm_open(name, O_CREAT | O_TRUNC | O_RDWR, 0666);
    ftruncate(shm_fd, size);
    void* ptr = mmap(0 , size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
        return -1;
    munmap(ptr, size);
    close(shm_fd);
}

int BasicFileSystemMemory::read_block(const char* name, unsigned long block_index, char *buffer){
    int shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1)
        return -1;
    void* ptr = mmap(0, getBlockSize(), PROT_READ, MAP_SHARED, shm_fd, block_index*getBlockSize());
    if (ptr == MAP_FAILED)
        return -1;
    sprintf(buffer, "%s", (char *)ptr);
    munmap(ptr, getBlockSize());
    close(shm_fd);
}

int BasicFileSystemMemory::write_block(const char* name, unsigned long block_index, const char *buffer){
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1)
        return -1;
    void* ptr = mmap(0, getBlockSize(), PROT_WRITE, MAP_SHARED, shm_fd, block_index*getBlockSize());
    if (ptr == MAP_FAILED)
        return -1;
    sprintf((char *)ptr, "%s", buffer);
    munmap(ptr, getBlockSize());
    close(shm_fd);
}