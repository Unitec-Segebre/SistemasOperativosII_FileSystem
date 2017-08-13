#include "basicFileSystemDisk.h"

using namespace std;

int BasicFileSystemDisk::format(const char* name, unsigned long size){
	ofstream outfile;
	outfile.open(name, ios::binary | ios::out | ios::trunc);
	outfile.seekp(size-1, ios::beg);
	outfile.write("", 1);
	outfile.close();
}

int BasicFileSystemDisk::read_block(const char* name, unsigned long block_index, char *buffer){
	ifstream infile;
	infile.open(name, ios::binary | ios::in);
	infile.seekg(block_index*getBlockSize(), ios::beg);
	infile.read(buffer, getBlockSize());
	infile.close();	
}

int BasicFileSystemDisk::write_block(const char* name, unsigned long block_index, const char *buffer){
	ofstream outfile;
	outfile.open(name, ios::binary | ios::out);
	outfile.seekp(block_index*getBlockSize(), ios::beg);
	outfile.write(buffer, getBlockSize());
	outfile.close();
}

int main(int argc, char const *argv[])
{
	BasicFileSystemDisk fs;
	fs.format((char*)"/home/segebre/Desktop/Mount/Test.txt", (unsigned long)fs.getBlockSize()*1024*800);
	char* buffer = (char*)calloc(1, fs.getBlockSize());
	strncpy(buffer, "Hola mi amigo!", fs.getBlockSize());
	fs.write_block((char*)"/home/segebre/Desktop/Mount/Test.txt", 0, buffer);
	char* buffer2 = new char[fs.getBlockSize()];
	fs.read_block((char*)"/home/segebre/Desktop/Mount/Test.txt", 0, buffer2);
	printf("%s\n", buffer2);
	return 0;
}