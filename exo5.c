#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int pipefd[2]; // Descripteurs de fichier pour le pipe
    pid_t pid;
    int nombre_mystere, proposition;

    // Création du pipe
    if (pipe(pipefd) == -1)
    {
        perror("Erreur lors de la création du pipe");
        exit(EXIT_FAILURE);
    }

    // Création du processus fils
    pid = fork();

    if (pid < 0)
    {
        perror("Erreur lors du fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {                                 // Processus fils (devine le nombre)
        close(pipefd[1]);             // Ferme l'extrémité d'écriture du pipe
        srand(time(NULL) ^ getpid()); // Initialisation aléatoire spécifique au fils

        while (1)
        {
            read(pipefd[0], &proposition, sizeof(int)); // Lit l'indice du père
            if (proposition == -1)
            {
                printf("[fils] J'ai trouvé le nombre mystère !\n");
                break;
            }

            proposition = rand() % 100 + 1;
            printf("[fils] Je propose %d\n", proposition);
            write(pipefd[0], &proposition, sizeof(int));
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {                     // Processus père (génère le nombre mystère)
        close(pipefd[0]); // Ferme l'extrémité de lecture du pipe
        srand(time(NULL));
        nombre_mystere = rand() % 100 + 1;
        printf("[pere] Nombre mystère choisi : %d\n", nombre_mystere);

        while (1)
        {
            write(pipefd[1], &nombre_mystere, sizeof(int));
            read(pipefd[1], &proposition, sizeof(int));

            if (proposition == nombre_mystere)
            {
                int fin = -1;
                write(pipefd[1], &fin, sizeof(int));
                break;
            }
            else if (proposition < nombre_mystere)
            {
                printf("[pere] C'est plus !\n");
            }
            else
            {
                printf("[pere] C'est moins !\n");
            }
        }
        close(pipefd[1]);
        wait(NULL);
        printf("*** FIN du processus < %d > ***\n", getpid());
    }

    return 0;
}