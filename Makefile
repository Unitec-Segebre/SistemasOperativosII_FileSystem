all:
	g++ basicFileSystem.cpp basicFileSystemDisk.cpp -o run
	sudo ./run

add:
	git add basicFileSystem.h basicFileSystem.cpp basicFileSystemDisk.h basicFileSystemDisk.cpp Makefile
