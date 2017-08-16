#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "FileSystem.h"

int main() {
    FileSystem fs;
    fs.create_disk("/home/segebre/Desktop/Mount/fstest.lol", fs.DISK, (unsigned long)4096*50);
    fs.add_file("/home/segebre/Desktop/Mount/fstest.lol", fs.DISK, "/home/segebre/Desktop/Mount/text.txt");
    return 0;
}