    if (qq < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        return 1;
    }
    const char msg[] = "hello";
    int r = mq_send(qq, msg, sizeof(msg) - 1, 0);
    if (r < 0) {
        fprintf(stderr, "send: %s\n", strerror(errno));
        return 1;
    }
    
    enum { BUFSIZE = 16384 };
    char buf[BUFSIZE];
    unsigned prio = 0;
    r = mq_receive(qq, buf, sizeof(buf), &prio);
    if (r < 0) {
        fprintf(stderr, "recv: %s\n", strerror(errno));
        return 1;
    }
    printf("received: %.*s, %u\n", r, buf, prio);
}
      
