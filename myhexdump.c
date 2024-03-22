#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>

#define OFFSET_WIDTH 8
#define LINE_BYTES 16
#define BUF_SIZE 4096

#define SPACE ' '
#define VERTICAL_BAR '|'
#define LF '\n'
#define DOT '.'
#define ASTERISK '*'

int mystrncmp(char a[], char b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] > b[i]) {
            return 1;
        }
        if (a[i] < b[i]) {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("arguments error\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    
    struct stat st;
    fstat(fd, &st);
    size_t total_size = st.st_size;

    int offset = 0;
    ssize_t total_read_size = 0;
    if (argc > 2) {
	// 支持-s选项
        offset = atoi(argv[2]);
        lseek(fd, offset, SEEK_SET);
        total_read_size += offset;
    }
    
    if (argc > 3) {
        int length = atoi(argv[3]);
        // 支持-n选项
        total_size = length < total_size ? length + offset: total_size;
    }

    ssize_t size;
    char buf[BUF_SIZE];
    char pre[LINE_BYTES] = { 0 };
    char cur[LINE_BYTES] = { 0 };
    // 支持*号
    bool repeat = false;

    while (total_read_size < total_size && (size = read(fd, buf, sizeof(buf))) > 0) {
        if (total_read_size + size > total_size) {
            size = total_size - total_read_size;
        }
        total_read_size += size;

        for (int i = 0; i < size; i++) {
            offset += 1;
            cur[i % LINE_BYTES] = buf[i];
    
            if (i % LINE_BYTES == LINE_BYTES - 1 || (i == size - 1 && total_read_size == total_size)) {
                // pre和cur可能包含空字符，不要使用strcmp/strncmp
                if (mystrncmp(pre, cur, LINE_BYTES) == 0) {
                    repeat = true;
                    // 当前行是最后一行，并且与前一行内容相同
                    if (i == size - 1 && total_read_size == total_size) {
                        printf("%c%c", ASTERISK, LF); 
                    }
                    continue;
                }
                
                // 如果在当前行之前，多行内容相同，只输出一个星号*
                if (repeat) {
                    printf("%c%c", ASTERISK, LF); 
                    repeat = false;
                }
        
                // 输出当前行的偏移量
                printf("%0*x", OFFSET_WIDTH, offset - i % LINE_BYTES - 1);
                
                // 输出十六进制数字
                for (int j = 0; j < LINE_BYTES; j++) {
                    // 每隔8字节额外输出一个空格
                    if (j % 8 == 0) {
                        printf("%c", SPACE);
                    }
        
                    if (j <= i % LINE_BYTES) {
                        int pos = (i / LINE_BYTES) * LINE_BYTES + j;
                        printf("%c%02x", SPACE, (unsigned char)buf[pos]);
                    } else {
                        // 如果内容不足一行，则填充空格
                        printf("%c%c%c", SPACE, SPACE, SPACE);
                    }
                }
    
                // 输出ASCII字符
                printf("%c%c%c", SPACE, SPACE, VERTICAL_BAR);
                for (int j = 0; j <= i % LINE_BYTES; j++) {
                    // 如果为可打印字符，isprint()声明在ctype.h中
                    int pos = (i / LINE_BYTES) * LINE_BYTES + j;
                    if (isprint(buf[pos])) {
                        printf("%c", buf[pos]);
                    } else {
                        printf("%c", DOT);
                    }
                }
                printf("%c%c", VERTICAL_BAR, LF);
                        
                // 更新前一行
                memcpy(pre, cur, sizeof(cur));
                memset(cur, 0, sizeof(cur));
            }
        }
    }

    // 末尾的偏移量
    printf("%0*x%c", OFFSET_WIDTH, offset, LF);

    close(fd); 
    return 0;
}
