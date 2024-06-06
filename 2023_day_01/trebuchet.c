#include<ctype.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>


#define BUF_SZ 65536

int main(int argc, char **argv) {
    int fdigit;
    int ldigit;
    int acc = 0;
    int state = 0;
    char buffer[BUF_SZ];
    ssize_t bytes_read;

    int fd = open("./long_input", O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    while((bytes_read = read(fd, buffer, BUF_SZ))) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            switch(buffer[i]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (state == 0) {
                    fdigit = ldigit = buffer[i] - '0';
                    state = 1;
                } else {
                    ldigit = buffer[i] - '0';
                    state = 2;
                }
                break;
            case '\n':
                acc += fdigit * 10 + ldigit;
                state = 0;
                break;
            default:
                break;
            }
        }
    }
    printf("%d\n", acc);
    return 0;
}
