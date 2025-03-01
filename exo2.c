#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5 // Nombre d'itérations

int globale = 1; // Variable globale

int random_wait(int min, int max)
{
    return min + rand() % (max - min + 1);
}

int main()
{
    pid_t pid;

    printf("[Processus : parent] PID = %d | PPID = %d | globale = %d\n", getpid(), getppid(), globale);
    fflush(stdout);

    pid = fork(); // Création du processus fils

    if (pid < 0)
    {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // Processus fils
        printf("[Processus : fils] PID = %d | PPID = %d | globale = %d\n", getpid(), getppid(), globale);
        fflush(stdout);
        for (int i = 1; i <= N; i++)
        {
            globale *= 2;
            srand(time(NULL) ^ getpid());                 // Mélange l’horloge et le PID
            int wait_time = random_wait(100000, 1000000); // Entre 100ms et 1s
            usleep(wait_time);
            printf("[Processus : fils] i = %d | globale = %d | durée = %d \n", i, globale, wait_time);
            fflush(stdout);
        }
        printf("*** FIN du processus < %d > | globale = %d ***\n", getpid(), globale);
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
    else
    { // Processus père
        printf("[Processus : parent] PID = %d | PPID = %d | globale = %d\n", getpid(), getppid(), globale);
        fflush(stdout);
        for (int i = 1; i <= N; i++)
        {
            globale += 10;
            srand(time(NULL) ^ getpid());                 // Mélange l’horloge et le PID
            int wait_time = random_wait(100000, 1000000); // Entre 100ms et 1s
            usleep(wait_time);
            printf("[Processus : père] i = %d | globale = %d | durée = %d \n", i, globale, wait_time);
            fflush(stdout);
        }
        wait(NULL); // Attente de la fin du processus fils
        printf("*** FIN du processus < %d > | globale = %d ***\n", getpid(), globale);
        fflush(stdout);
    }

    return 0;
}
