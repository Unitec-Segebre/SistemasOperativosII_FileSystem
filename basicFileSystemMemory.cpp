#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "basicFileSystemMemory.h"

using namespace std;

int BasicFileSystemMemory::format(const char* name, unsigned long size){
	int shm_fd = shm_open(name, O_CREAT | O_TRUNC | O_RDWR, 0666);
	ftruncate(shm_fd, size);
	void* ptr = mmap(0 , size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	printf("%X\n", ptr);
	if (ptr == MAP_FAILED)
		return -1;
	munmap(ptr, size);
	close(shm_fd);
}

int BasicFileSystemMemory::read_block(const char* name, unsigned long block_index, char *buffer){
	int shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1)
		return -1;
	void* ptr = mmap(0, (block_index+1)*getBlockSize(), PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED)
		return -1;
	sprintf(buffer, "%s", (char *)ptr+block_index*getBlockSize());
	munmap(ptr, getBlockSize());
	close(shm_fd);
}

int BasicFileSystemMemory::write_block(const char* name, unsigned long block_index, const char *buffer){
	printf("%d\n", 1);
	int shm_fd = shm_open(name, O_RDWR, 0666);
	printf("%d\n", 2);
	if (shm_fd == -1)
		return -1;
	printf("%d\n", 3);
	void* ptr = mmap(0, getBlockSize(), PROT_WRITE, MAP_SHARED, shm_fd, block_index*getBlockSize());
	printf("%d\n", 4);
	if (ptr == MAP_FAILED)
		return -1;
	printf("%d\n", 5);
	printf("%X\n", ptr);
	sprintf((char *)ptr, "%s", buffer);
	printf("%d\n", 6);
	munmap(ptr, getBlockSize());
	printf("%d\n", 7);
	close(shm_fd);
	printf("%d\n", 8);
}

int main(int argc, char const *argv[])
{
	BasicFileSystemMemory fs;
	if(fs.format((char*)"Test.txt", (unsigned long)fs.getBlockSize()) == 1){
		printf("%s\n", "ERR");
		return 0;
	}
	char* buffer = (char*)calloc(1, fs.getBlockSize());
	strncpy(buffer, "Hola mi amigo!", fs.getBlockSize());
	if (fs.write_block((char*)"Test.txt", 0, buffer) == -1){
	printf("%s\n", "ERR");
		return 0;
	}
	char* buffer2 = new char[fs.getBlockSize()];
	if (fs.read_block((char*)"Test.txt", 0, buffer2) == -1){
		printf("%s\n", "ERR");
		return 0;
	}
	printf("%s\n", buffer2);
	return 0;
}