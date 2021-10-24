#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

/**
 * MAXNAMLEN定义在dirent.h<br>
 * 函数getcwd()定义在unistd.h
 */
int main() {
    char buffer[MAXNAMLEN];
    getcwd(buffer, sizeof(buffer));
    printf("%s\n", buffer);
    return 0;
}