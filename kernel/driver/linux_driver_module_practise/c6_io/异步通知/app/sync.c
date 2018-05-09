#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

int fd;

int g_running;

void sync_handler(int num) {
    printf("recv sync single!\n");
}


void stop_handler(int signum) {
    close(fd);
    g_running = 0;
    printf("stop prom\n");
    exit(0);
}


main() {
    int oflags;

    g_running = 1;

    signal(SIGINT, stop_handler);

    fd = open("/dev/sync", O_RDWR); //,S_IRUSR | S_IWUSR);

    if (fd > 0) {
        printf("open /dev/sync ok\n");
    } else {
        printf("open /dev/sync fail\n");
    }

    signal(SIGIO, sync_handler);

    fcntl(fd, F_SETOWN, getpid());

    oflags = fcntl(fd, F_GETFL);

    fcntl(fd, F_SETFL, oflags | FASYNC);

    while (g_running) {
        sleep(100);
    }
}
















