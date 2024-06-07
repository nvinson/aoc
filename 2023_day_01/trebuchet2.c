#include<ctype.h>
#include<errno.h>
#include<fcntl.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define BUF_SZ 65536

struct BufferedReader {
    int fd;
    ssize_t sz;
    ssize_t cap;
    char *buffer;
};

ssize_t fdgetdelim(char **line, size_t *n, int delim, struct BufferedReader *br) {
    size_t llen = 0;
    size_t lcap;

    if (n == NULL || br == NULL) {
        errno = EINVAL;
        return -errno;
    }

    if (*line == NULL) {
        lcap = 128;
        *line = calloc(lcap, sizeof(**line));
        if (*line == NULL) {
            return -errno;
        }
    } else if (*n < 0) {
        errno = EINVAL;
        return -errno;
    } else {
        lcap = *n;
        **line = '\0';
    }

    while(1) {
        for(ssize_t i = 0; i < br->sz; i++) {
            if (br->buffer[i] == delim) {
                if (SIZE_MAX - i + 1  <= llen) {
                    free(*line);
                    errno = ENOMEM;
                    return -errno;
                }
                if (llen + i + 1 >= lcap) {
                    while (lcap <= llen + i + 1) {
                        size_t new_lcap = lcap << 1;
                        if (new_lcap <= lcap) {
                            free(*line);
                            errno = ENOMEM;
                            return -errno;
                        }
                        lcap = new_lcap;
                    }
                    char *tmp = realloc(*line, lcap);
                    if (tmp == NULL) {
                        free(*line);
                        errno = ENOMEM;
                        return -errno;
                    }
                    *line = tmp;
                }
                strncat(*line, br->buffer, i + 1);
                llen += i + 1;
                memmove(br->buffer, br->buffer + i + 1, br->sz -= i + 1);
                *n = llen;
                return llen;
            }
        }
        if (SIZE_MAX - br->sz <= llen) {
            free(*line);
            errno = ENOMEM;
            return -errno;
        }
        while (lcap <= llen + br->sz) {
            lcap <<= 1;
            if (!lcap) {
                free(*line);
                errno = ENOMEM;
                return -errno;
            }
        }
        strncat(*line + llen, br->buffer, br->sz);
        llen += br->sz;
        br->sz = read(br->fd, br->buffer, br->cap);
        if (br->sz < 0) {
            return -errno;
        }
        if (br->sz == 0) {
            *n = llen;
            return llen;
        }
    }
}

ssize_t fdgetline(char **line, size_t *n, struct BufferedReader *br) {
    return fdgetdelim(line, n, '\n', br);
}

int main(int argc, char **argv) {
    char *line = NULL;
    size_t llen = 0;
    size_t acc = 0;

    int fd = open("./long_input", O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    struct BufferedReader br = {
        .fd = fd,
        .sz = 0,
        .cap = BUF_SZ,
        .buffer = calloc(BUF_SZ, sizeof(*br.buffer)),
    };

    if (br.buffer == NULL) {
        perror("calloc");
    }

    while (fdgetline(&line, &llen, &br) > 0) {
        size_t i;
        size_t j;
        int fdigit;
        int ldigit;
        for (i = 0, fdigit=-1; fdigit == -1 && i < llen; i++) {
            if ('1' <= line[i] && line[i] <= '9') {
                fdigit = line[i] - '0';
            } else {
                if (llen - i > 2) {
                    if (!strncmp("one", line + i, 3)) {
                        fdigit = 1;
                    }
                    if (!strncmp("two", line + i, 3)) {
                        fdigit = 2;
                    }
                    if (!strncmp("six", line + i, 3)) {
                        fdigit = 6;
                    }
                }
                if (llen - i > 3) {
                    if (!strncmp("four", line + i, 4)) {
                        fdigit = 4;
                    }
                    if (!strncmp("five", line + i, 4)) {
                        fdigit = 5;
                    }
                    if (!strncmp("nine", line + i, 4)) {
                        fdigit = 9;
                    }
                }
                if (llen - i > 4) {
                    if (!strncmp("three", line + i, 5)) {
                        fdigit = 3;
                    }
                    if (!strncmp("seven", line + i, 5)) {
                        fdigit = 7;
                    }
                    if (!strncmp("eight", line + i, 5)) {
                        fdigit = 8;
                    }
                }
            }
        }
        for (j = llen - 2, ldigit=-1; ldigit == -1 && j >= i; j--) {
            if ('1' <= line[j] && line[j] <= '9') {
                ldigit = line[j] - '0';
            } else {
                if (j > 2) {
                    if (!strncmp("one", line + j - 2, 3)) {
                        ldigit = 1;
                    }
                    if (!strncmp("two", line + j - 2, 3)) {
                        ldigit = 2;
                    }
                    if (!strncmp("six", line + j - 2, 3)) {
                        ldigit = 6;
                    }
                }
                if (j > 3) {
                    if (!strncmp("four", line + j - 3, 4)) {
                        ldigit = 4;
                    }
                    if (!strncmp("five", line + j - 3, 4)) {
                        ldigit = 5;
                    }
                    if (!strncmp("nine", line + j - 3, 4)) {
                        ldigit = 9;
                    }
                }
                if (j > 4) {
                    if (!strncmp("three", line + j - 4, 5)) {
                        ldigit = 3;
                    }
                    if (!strncmp("seven", line + j - 4, 5)) {
                        ldigit = 7;
                    }
                    if (!strncmp("eight", line + j - 4, 5)) {
                        ldigit = 8;
                    }
                }
            }
        }
        acc += fdigit * 10 + (ldigit <= 0 ? fdigit : ldigit);
    }
    printf("acc: %zd\n", acc);
    free(line);
    free(br.buffer);
    return 0;
}
