#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void c(char *buffer, const char *key, int l) {
    int i, j = 0;
    int max = strlen(key);
    for(i=0; i<l; i++) {
        *(buffer + i) = *(buffer + i) ^ *(key + j);
        j = (j + 1) % max;
    }
}

int main(int argc, char *argv[]) {
    int fd;
    int fd_dest;
    int i;
    char buffer[BUFFER_SIZE];
    if (argc<3) {
        printf("format: crypt <key> <file>\n");
        return -1;
    }
    if((fd = open(argv[2], O_RDONLY))==-1) {
        perror("Source open error");
        return -1;
    }

    if((fd_dest = open(strncat(argv[2], ".crypt", 7), O_WRONLY | O_CREAT, 0666))==-1) {
        perror("Destination open error");
        return -1;
    }

    while((i = read(fd, buffer, BUFFER_SIZE))!=0) {
        if(i==-1) {
            perror("Read error");
            return -1;
        }
        c(buffer, argv[1], i);
        if(write(fd_dest, buffer, i)==-1) {
            perror("Write error");
            return -1;
        }
    }

    if(close(fd)==-1) {
        perror("Source close error");
        return -1;
    }

    if(close(fd_dest)==-1) {
        perror("Destination close error");
        return -1;
    }

    return 0;
}
