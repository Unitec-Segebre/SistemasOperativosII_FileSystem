#include "FileSystem.h"

struct metadata
{
	unsigned long disk_size;
	unsigned long disk_block_count;
	unsigned long bitmap_block_start;
	unsigned long bitmap_bit_count;
	unsigned long bitmap_block_count;
	unsigned long bitmap_block_end;
	unsigned long master_directory_block_start;
	unsigned long master_directory_file_count;
};

int FileSystem::format(const char* name, unsigned long size, storage_t type){
	if (size <= 0){
		printf("%s\n", "Size not acceptable");
		return -1;
	}

	BasicFileSystem* fs;
	if (type == DISK)
		fs = new BasicFileSystemDisk();
	else if(type == DISK)
		fs = new BasicFileSystemMemory();
	else{
		printf("%s\n", "Please specify type");
		return 0;
	}

	struct metadata fs_metadata;
	fs_metadata.disk_block_count = ((size-1)/fs->getBlockSize())+1;
	if (fs_metadata.disk_block_count < 3){
		printf("%s\n", "Not enough blocks");
	}
	fs_metadata.disk_size = fs_metadata.disk_block_count*fs->getBlockSize();
	fs_metadata.bitmap_block_start = 2;
	fs_metadata.bitmap_bit_count = fs_metadata.disk_block_count;
	fs_metadata.bitmap_block_count = (((fs_metadata.bitmap_bit_count-1)/8)/fs->getBlockSize())+1;
	fs_metadata.bitmap_block_end = fs_metadata.bitmap_block_start + fs_metadata.bitmap_block_count - 1;
	fs_metadata.master_directory_block_start = fs_metadata.bitmap_block_end + 1;
	fs_metadata.master_directory_file_count = 0;

	printf("disk_size: %lu\n", fs_metadata.disk_size);
	printf("bitmap_block_start: %lu\n", fs_metadata.bitmap_block_start);
	printf("bitmap_bit_count: %lu\n", fs_metadata.bitmap_bit_count);
	printf("bitmap_block_count: %lu\n", fs_metadata.bitmap_block_count);
	printf("bitmap_block_end: %lu\n", fs_metadata.bitmap_block_end);
	printf("master_directory_block_start: %lu\n", fs_metadata.master_directory_block_start);
	printf("master_directory_file_count: %lu\n", fs_metadata.master_directory_file_count);

	char* buffer = (char*)calloc(1, fs->getBlockSize());

	//format disk
	fs->format(name, fs_metadata.disk_size);

	//write metadata
	memcpy(buffer, &fs_metadata, sizeof(struct metadata));
	printf("%lu\n", (fs_metadata.disk_size));
	printf("%lu\n", *((unsigned long*)(buffer+(sizeof(unsigned long)*0))));
	fs->write_block(name, fs_metadata.bitmap_block_start-1, buffer);

	//write bitmap
	free(buffer);
	buffer = (char*)calloc(1, fs->getBlockSize());
	setBit(&(buffer[0]), 0, 1);
	setBit(&(buffer[0]), 1, 1);
	int i;
	for (i = 2; i <= fs_metadata.bitmap_block_count + 2; i++){
		setBit(&(buffer[(i/8)%fs->getBlockSize()]), i%8, 1);
		// if(/*buffer[(i/8)%fs->getBlockSize()] > 0*/ false){
		// 	for (int j = 0; j < 8; j++) {
		//       printf("%d", !!((buffer[(i/8)%fs->getBlockSize()] << j) & 0x80));
		// 	}
		// 	printf("\n");
		// }
		if(i%fs->getBlockSize()==0){
			fs->write_block(name, fs_metadata.bitmap_block_start+((i/8)/fs->getBlockSize()), buffer);
			free(buffer);
			buffer = (char*)calloc(1, fs->getBlockSize());
		}
	}
	fs->write_block(name, fs_metadata.bitmap_block_start+((i/8)/fs->getBlockSize()), buffer);
	free(buffer);
	
}

int FileSystem::getBit(char sequence, int bitNumber){
	return (sequence<<(7-bitNumber))>>((7-bitNumber)+bitNumber);
}

int FileSystem::setBit(char* sequence, int bitNumber, char value){
	if (value)
		*sequence |= (1u<<bitNumber);
	else
		*sequence &= ~(1u<<bitNumber);
}

int main(int argc, char const *argv[])
{
	FileSystem fs;
	fs.format("/home/segebre/Desktop/Mount/fstest.lol", (unsigned long)4096*1024*512, fs.DISK);
	return 0;
}