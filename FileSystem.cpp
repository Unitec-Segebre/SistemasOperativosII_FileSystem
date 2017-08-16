//
// Created by segebre on 8/14/17.
//

#include "FileSystem.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

struct fs_metadata
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

struct file_metadata
{
    char name[48];
    unsigned long size;
    unsigned long block_start;
};

int FileSystem::create_disk(const char* name, storage_t type, unsigned long size){
    if (size <= 0){
        printf("%s\n", "Size not acceptable");
        return -1;
    }

    BasicFileSystem* fs;
    if (type == DISK)
        fs = new BasicFileSystemDisk();
    else if(type == MEMORY)
        fs = new BasicFileSystemMemory();
    else{
        printf("%s\n", "Please specify type");
        return 0;
    }

    struct fs_metadata fs_metadata;
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

//    printf("disk_size: %lu\n", fs_metadata.disk_size);
//    printf("bitmap_block_start: %lu\n", fs_metadata.bitmap_block_start);
//    printf("bitmap_bit_count: %lu\n", fs_metadata.bitmap_bit_count);
//    printf("bitmap_block_count: %lu\n", fs_metadata.bitmap_block_count);
//    printf("bitmap_block_end: %lu\n", fs_metadata.bitmap_block_end);
//    printf("master_directory_block_start: %lu\n", fs_metadata.master_directory_block_start);
//    printf("master_directory_file_count: %lu\n", fs_metadata.master_directory_file_count);

    char* buffer = (char*)calloc(1, fs->getBlockSize());

    //format disk
    fs->format(name, fs_metadata.disk_size);

    //write fs_metadata
    memcpy(buffer, &fs_metadata, sizeof(struct fs_metadata));
    // printf("%lu\n", (fs_metadata.disk_size));
    // printf("%lu\n", *((unsigned long*)(buffer+(sizeof(unsigned long)*0))));
    fs->write_block(name, fs_metadata.bitmap_block_start-1, buffer);

    //write bitmap
    free(buffer);
    buffer = (char*)calloc(1, fs->getBlockSize());
    setBit(&(buffer[0]), 0, 1);
    setBit(&(buffer[0]), 1, 1);
    int i;
    for (i = fs_metadata.bitmap_block_start; i <= fs_metadata.master_directory_block_start; i++){
        setBit(&(buffer[(i/8)%fs->getBlockSize()]), i%8, 1);
        // if(/*buffer[(i/8)%fs->getBlockSize()] > 0*/ true){
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
//    printf("NEXT BLOCK!!!: %lu\n", getFreeBlock(name, fs));
    delete fs;

}

int FileSystem::add_file(const char* name, storage_t type, const char* file_name){
    BasicFileSystem* fs;
    if (type == DISK)
        fs = new BasicFileSystemDisk();
    else if(type == MEMORY)
        fs = new BasicFileSystemMemory();
    else{
        printf("%s\n", "Please specify type");
        return 0;
    }

    //leemos fs_metadata
    struct fs_metadata fs_metadata;
    char* buffer;
    buffer = (char*)calloc(1, fs->getBlockSize());
    fs->read_block(name, 1, buffer);
    memcpy(&fs_metadata, buffer, sizeof(struct fs_metadata));
    if(fs_metadata.master_directory_file_count*sizeof(struct file_metadata) >= fs->getBlockSize()){
        printf("%s\n", "No more space");
        return 0;
    }

    //leemos el master directory
    free(buffer);
    buffer = (char*)calloc(1, fs->getBlockSize());
    fs->read_block(name, fs_metadata.master_directory_block_start, buffer);

    //chequeamos que no exista un archivo con mismo nombre
    for(int i = 0; i < fs_metadata.master_directory_file_count; i++){
        // if(buffer[i*sizeof(struct file_metadata)] == '\0')
        // 	break;
        // else if((i+1)*sizeof(struct file_metadata) > fs->getBlockSize()){
        // 	printf("%s\n", "No more space");
        // 	return 0;
        // }
        if(!strncmp(file_name, &(buffer[i*sizeof(struct file_metadata)]), 48)){
            printf("%s\n", "File already exists");
            return 0;
        }
    }

    //abrimos el archivo a copiar
    ifstream infile;
    infile.open(file_name, ios::binary | ios::in);
    if(!infile){
        printf("%s\n", "opening cp file err");
        return -1;
    }
    infile.seekg(0, ios::end);

    //escribimos file_metadata
    struct file_metadata file_metadata;
    memcpy(file_metadata.name, file_name, 48);
    file_metadata.block_start = getFreeBlock(name, fs);
    file_metadata.size = infile.tellg();
    printf("%lu", file_metadata.size);
    memcpy(&(buffer[fs_metadata.master_directory_file_count]), &file_metadata, sizeof(struct file_metadata));
    fs->write_block(name, fs_metadata.master_directory_block_start, buffer);

    //regresamos el apuntador de archivo al inicio
    infile.seekg(0, ios::beg);

    //update bitmap and metadata[master directory count]
    reserveFreeBlock(name, fs, file_metadata.block_start);
    fs_metadata.master_directory_file_count++;
    free(buffer);
    buffer = (char*)calloc(1, fs->getBlockSize());
    memcpy(buffer, &fs_metadata, sizeof(struct fs_metadata));
//    printf("%lu\n", *((unsigned long*)&(fs_metadata)));
//    printf("%lu\n", ((struct fs_metadata*)(buffer))->master_directory_block_start);
    fs->write_block(name, 1, buffer);

    //write from file to disk
    unsigned long blocks_needed = file_metadata.size/fs->getBlockSize();
    unsigned long index_count_per_block = fs->getBlockSize()/sizeof(unsigned long);
    unsigned long* buffer_index1 = (unsigned long*)calloc(1, fs->getBlockSize());
    for (int i = 0; i <= blocks_needed/index_count_per_block; i++){
        buffer_index1[i] = reserveFreeBlock(name, fs, getFreeBlock(name, fs));
        unsigned long* buffer_index2 = (unsigned long*)calloc(1, fs->getBlockSize());
        for (int j = 0; j < index_count_per_block && j <= blocks_needed-i*index_count_per_block; j++){
            buffer_index2[j] = reserveFreeBlock(name, fs, getFreeBlock(name, fs));
            free(buffer);
            buffer = (char*)calloc(1, fs->getBlockSize());
            if (j <  blocks_needed-i*index_count_per_block)
                infile.read(buffer, fs->getBlockSize());
            else
                infile.read(buffer, file_metadata.size-infile.tellg());
            fs->write_block(name, buffer_index2[j], buffer);
            printf(buffer);
        }
        fs->write_block(name, buffer_index1[i], (const char*)buffer_index2);
    }
    fs->write_block(name, file_metadata.block_start, (const char*)buffer_index1);

    infile.close();

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

unsigned long FileSystem::getFreeBlock(const char* name, BasicFileSystem* fs){
    struct fs_metadata fs_metadata;
    char* buffer;

    buffer = (char*)calloc(1, fs->getBlockSize());
    fs->read_block(name, 1, buffer);
    memcpy(&fs_metadata, buffer, sizeof(struct fs_metadata));
    for (int current_block = fs_metadata.bitmap_block_start; current_block <= fs_metadata.bitmap_block_end; current_block++){
        free(buffer);
        buffer = (char*)calloc(1, fs->getBlockSize());
        fs->read_block(name, current_block, buffer);
        for (int current_byte = 0; current_byte < fs->getBlockSize(); current_byte++){
            for (int current_bit = 0; current_bit < 8; current_bit++) {
                if (!getBit(buffer[current_byte], current_bit)){
                    return ((current_block-fs_metadata.bitmap_block_start)*fs->getBlockSize()*8)+(current_byte*8)+current_bit;
                }
            }
        }
    }

    free(buffer);
}

unsigned long FileSystem::reserveFreeBlock(const char* name, BasicFileSystem* fs, unsigned long block_index){
    struct fs_metadata fs_metadata;
    char* buffer;

    buffer = (char*)calloc(1, fs->getBlockSize());
    fs->read_block(name, 1, buffer);
    memcpy(&fs_metadata, buffer, sizeof(struct fs_metadata));
    if (fs_metadata.bitmap_block_start + ((block_index/8)/fs->getBlockSize()) <= fs_metadata.bitmap_block_end){
        free(buffer);
        buffer = (char*)calloc(1, fs->getBlockSize());
        fs->read_block(name, fs_metadata.bitmap_block_start + ((block_index/8)/fs->getBlockSize()), buffer);
        setBit(&(buffer[(block_index/8)%fs->getBlockSize()]), block_index%8, 1);
        fs->write_block(name, fs_metadata.bitmap_block_start + ((block_index/8)/fs->getBlockSize()), buffer);
    }
    else
        printf("%s\n", "Not enough space");

    free(buffer);
    return block_index;
}