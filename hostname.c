#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PROGRAM_NAME "hostname"
#define ROOT_UID 0

int main(int argc, char *argv[]) {

    int host_name_len = sysconf(_SC_HOST_NAME_MAX);
    char buff[host_name_len];
    if (argc == 1) {
        gethostname(buff, sizeof(buff));
        printf("%s\n", buff);
    } else if (argc == 2) {
        // super-user: root
        if (getuid() == ROOT_UID) {
            strcpy(buff, argv[1]);
            sethostname(buff, sizeof(buff));
        } else {
            printf("%s: sethostname: Operation not permitted\n", PROGRAM_NAME);
        }
    } else {
        printf("usage: %s [name-of-host]\n", PROGRAM_NAME);
    }
    return 0;
}