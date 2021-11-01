#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>

/**
 * MAXPATHLEN定义在sys/param.h<br>
 * 函数getcwd()定义在unistd.h
 */
int main() {
    char buff[MAXPATHLEN];
    getcwd(buff, sizeof(buff));
    printf("%s\n", buff);
    return 0;
}