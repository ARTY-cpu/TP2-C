#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Fonction pour générer un temps d'attente aléatoire entre 1 et 5 secondes
int random_wait_time()
{
    return 1 + rand() % 5;
}

int main()
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    pid_t pid;

    printf("[pere] PID = %d | PPID = %d\n", getpid(), getppid());

    pid = fork();

    if (pid < 0)
    {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // Processus fils
        int sleep_time = random_wait_time();
        printf("[fils] PID = %d | PPID = %d\n", getpid(), getppid());
        printf("[fils] Je m’endors pour %ds...\n", sleep_time);
        sleep(sleep_time);
        printf("[fils] Je suis reveille !\n");
        printf("*** FIN du processus < %d > ***\n", getpid());
        exit(EXIT_SUCCESS);
    }
    else
    { // Processus père
        printf("[pere] En attente du reveil de son fils...\n");
        wait(NULL); // Attente de la fin du processus fils
        printf("[pere] Mon fils s est reveille !\n");
        printf("*** FIN du processus < %d > ***\n", getpid());
    }

    return 0;
}
