#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define FILE_LENGTH 0x400 /* 1024 */

int main(int argc, char *argv[])
{
    int fd;
    void *map_memory;

    if(argc < 2) {
        printf("Usage: ./a.out <string>\n");
        return 1;
    }

    /*
     * Open a file to be mapped.
     * S_IRUSR: read permission, owner
     * S_IWUSR: write permission, owner
     */
    fd = open("hw4.test", O_RDWR | O_CREAT, //測試檔在此
        S_IRUSR | S_IWUSR);
        lseek(fd, FILE_LENGTH-1, SEEK_SET);
        write(fd, "", 1);
        lseek(fd, 0, SEEK_SET);

    /* Create map memory. */
    map_memory = mmap(0, FILE_LENGTH,
        PROT_WRITE, MAP_SHARED,
        fd, 0);
    
    close(fd);

    /* Write to mapped memory. */
    if(strlen(argv[1]) < FILE_LENGTH)
        sprintf((char*)map_memory, "%s", argv[1]);

    munmap(map_memory, FILE_LENGTH);
    return 0;
}