#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


enum 
{
    SIZE_OF_FILE = 64,
    BYTE_IN_KILO = 1024,
    BITS_FOR_OFFSET = 9,
    PPN = 0xfe00,
    OFFSET = 0x1ff
};


int
main(int argc, char *argv[])
{
    int filedes = open(argv[1], O_RDONLY);
    if (filedes == -1) {
        fprintf(stderr, "Open error!\n");
        return 1;
    }
    size_t size = SIZE_OF_FILE * BYTE_IN_KILO;
    void *ptr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, filedes, 0);
    if (ptr == MAP_FAILED) {
        exit(1);
    }
    close(filedes);
    int tlb_addres;
    sscanf(argv[2], "%x", &tlb_addres);
    unsigned int virtual_addres;
    while (scanf("%x", &virtual_addres) > 0) {
        unsigned short *virtual_ptr = (unsigned short *) (ptr + tlb_addres + (virtual_addres >> BITS_FOR_OFFSET) * 2);
        unsigned short PPN_num = *virtual_ptr & PPN;
        unsigned short phys_addr = PPN_num | (virtual_addres & OFFSET);
        unsigned short answer = *(unsigned short *)(ptr + phys_addr);
        printf("%u\n", answer); 
    }
    munmap(ptr, size);
}
