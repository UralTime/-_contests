#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

enum { BYTES_IN_SHORT = 2 };

int
main(int argc, char *argv[])
{
    if (argc <= 1)
        return 1;
    int file = open(argv[1], O_RDONLY);
    if (file == -1)
        return 1;
    unsigned short min;
    unsigned char bytes[BYTES_IN_SHORT], is_min = 0;
    while (read(file, bytes, sizeof(bytes)) == sizeof(bytes)) {
        if (bytes[1] % 2 == 0) {
            unsigned char tmp = bytes[0];
            bytes[0] = bytes[1];
            bytes[1] = tmp;
            unsigned short *number = (unsigned short *) bytes;
            if (is_min == 0 || *number < min) { // thanks to lazy logic
                is_min = 1;
                min = *number;
            }
            if (min == 0) {
                printf("%u\n", min);
                return 0;
            }
        }    
    }
    if (is_min)
        printf("%u\n", min);
    close(file);
}
