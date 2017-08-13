all:
	g++ basicFileSystem.cpp basicFileSystemDisk.cpp -o run
	./run

add:
	git add basicFileSystem.h basicFileSystem.cpp basicFileSystemDisk.h basicFileSystemDisk.cpp Makefile
