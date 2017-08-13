all:
	g++ basicFileSystem.cpp basicFileSystemDisk.cpp -o run
	sudo ./run

mem:
	g++ basicFileSystem.cpp basicFileSystemMemory.cpp -o run -lrt
	./run

add:
	git add basicFileSystem.h basicFileSystem.cpp basicFileSystemDisk.h basicFileSystemDisk.cpp basicFileSystemMemory.h basicFileSystemMemory.cpp Makefile
