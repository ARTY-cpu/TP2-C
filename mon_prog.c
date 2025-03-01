#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("[fils] PID = %d | PPID = %d\n", getpid(), getppid());
    printf("[fils] commande de lancement : %s\n", argv[0]);
    if (argc > 1) {
        printf("[fils] %s\n", argv[1]);
    }
    return 0;
}
