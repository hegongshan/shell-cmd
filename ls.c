#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

void printDir(char *name);

/**
 * 函数getcwd()定义在unistd.h中 <br>
 * 常量MAXNAMLEN，函数opendir()、readdir()以及closedir()定义在dirent.h中
 */
int main(int argc, char *argv[]) {
    char buff[MAXNAMLEN];
    if (argc == 1) {
        getcwd(buff, sizeof(buff));
        printDir(buff);
    } else {
        for (int i = 1; i < argc; ++i) {
            printf("%s:\n", argv[i]);
            if (argc != '.') {
                printDir(argv[i]);
            } else {
                getcwd(buff, sizeof(buff));
                printDir(buff);
            }
            putchar('\n');
        }
    }
    return 0;
}

void printDir(char *name) {
    DIR *dp = opendir(name);
    if (!dp) {
        fprintf(stderr, "open directory error!\n");
        return ;
    }
    struct dirent *filename;
    int idx = 0;
    while ((filename = readdir(dp))) {
        // ignore dot file
        if (filename->d_name[0] != '.') {
            if (idx > 0) {
                putchar('\t');
            }
            printf("%s", filename->d_name);
            idx ++;
        }
    }
    putchar('\n');
    closedir(dp);
}