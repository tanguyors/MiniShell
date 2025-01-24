
#include "../include/minishell.h"
# include <unistd.h> 

/* A faire : -Intégration de la variable $? permettant de voir la dernière sortie des commandes effectuées. 
			 -Tester la robustesse du code, faire un rapport des erreurs.
			 -Checker les leaks. <-- actuellement en cours
			 -Remise à la norme. */

/* Erreur :  -gérer le free(input) avec les signaux + ft_exit() */
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

static void signals()
{
    struct sigaction sa;

    sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

int main(void)
{
    struct s_shell value;
    struct s_shell *head;
    char *rl_input;

    signals();
    ascii_art();
    while (1)
    {
        head = NULL;
        rl_input = readline("minishell> ");
        if (rl_input != NULL)       // Permet d'avoir un historique cmd
            add_history(rl_input);
        if (rl_input == NULL)  // Permet d'exit le shell (ctrl + D)
        {
            free(rl_input);
            exit(EXIT_SUCCESS);
        }
        head = parsing(rl_input, head);
		parse_execution(head, rl_input);
        printf("test exit\n");
        free(rl_input);
        if (head)
        {
            print_list(head);
			print_token(head);
            free_list(head);
        }
    }
    return (0);
}
