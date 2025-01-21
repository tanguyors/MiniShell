
#include "../include/minishell.h"
# include <unistd.h> 

/* A faire : -Intégration de la variable $? permettant de voir la dernière sortie des commandes effectuées. 
			 -Intégration des redirections. <-- actuellement en cours
			 -Tester la robustesse du code, faire un rapport des erreurs.
			 -Checker les leaks.
			 -Remise à la norme. */

/* Erreur : -Clear (TERM environment variable not set.) */
static void ascii_art()
{
    printf("\n\n\n ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓ \n");
    printf("▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n");
    printf("▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n");
    printf("▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n");
    printf("▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n");
    printf("░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n");
    printf("░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n");
    printf("░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n");
    printf("       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n\n\n");
}

int main(void)
{
    struct s_shell value;
    struct s_shell *head;
    struct sigaction sa;
    char **tokens;
    char *input;

    sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    ascii_art();
    while (1)
    {
        head = NULL;
        input = readline("minishell> ");
        if (input != NULL)       // Permet d'avoir un historique cmd
            add_history(input);
        if (input == NULL)  // Permet d'exit le shell (ctrl + D)
            exit(EXIT_SUCCESS);
        head = parsing(input, head);
		parse_execution(head);
        if (head)
        {
            print_list(head);
			print_token(head);
            free_list(head);
        }
        free(input);
    }
    return (0);
}
