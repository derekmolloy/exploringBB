#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define MMAP_LOC   "/sys/class/uio/uio0/maps/map1/"

unsigned int readFileValue(char filename[]){
   FILE* fp;
   unsigned int value = 0;
   fp = fopen(filename, "rt");
   fscanf(fp, "%x", &value);
   fclose(fp);
   return value;
}

int main(int argc, char **argv) {
    int fd;
    void *map_base, *virt_addr;
    unsigned long read_result, writeval;

    unsigned int addr = readFileValue(MMAP_LOC "addr");
    unsigned int dataSize = readFileValue(MMAP_LOC "size");

    off_t target = addr;

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1){
	printf("Failed to open memory!");
	return -1;
    }
//   printf("/dev/mem opened.\n");
    fflush(stdout);

    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) {
       printf("Failed to map base address");
       return -1;
    }
//    printf("Memory mapped at address %p.\n", map_base);
    fflush(stdout);
    int i=0;
    for(i=0; i<1000; i++){
	virt_addr = map_base + (target & MAP_MASK);
        read_result = *((unsigned long *) virt_addr);
        //printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result);
        printf("%d %d\n",i, read_result);
        target+=4;
    }
    fflush(stdout);

    if(munmap(map_base, MAP_SIZE) == -1) {
       printf("Failed to unmap memory");
       return -1;
    }
    close(fd);
    return 0;
}
