int 
main(int argc, char *argv[])
{
    int fd = read(argv[1], RD_ONLY, 0);
    if (fd < 0)
        exit(1);
    if (stb.st_size != 0) {
        // st_size: off_t
        size_t z = stb.st_size;
        if (z != stb.st_size) {
            fprintf(stderr, "file too big\n");
            exit(1);
        }
     
        unsigned char *ptr = mmap(NULL, z, PROT_READ, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED) {
            fprintf(stderr, "mmap failed\n");
            exit(1);
        }
        close(file);
    }
    return 0;
}
