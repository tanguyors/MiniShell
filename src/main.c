
#include "../include/minishell.h"
# include <unistd.h> 

/* A faire : -Intégration de la variable $? permettant de voir la dernière sortie des commandes effectuées. <-- actuellement en cours
			 -Tester la robustesse du code, faire un rapport des erreurs.
			 -Checker les leaks.
			 -Remise à la norme. */

/* Erreur :  -gérer "cat << eof | grep a" */
static void ascii_art()
{
    printf("\n\n\n");
    printf("███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓     \n");
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
    struct s_shell shell;
    struct s_shell *head;

    signals();
    //ascii_art();
    while (1)
    {
        head = NULL;
        shell.rl_input = readline("minishell> ");
        if (shell.rl_input != NULL)       // Permet d'avoir un historique cmd
            add_history(shell.rl_input);
        if (shell.rl_input == NULL)  // Permet d'exit le shell (ctrl + D)
        {
            free(shell.rl_input);
            exit(EXIT_SUCCESS);
        }
        head = parsing(shell.rl_input, head);
		parse_execution(&shell, head);
        free(shell.rl_input);
        if (head)
        {
            //print_list(head);
			//print_token(head);
            free_list(head);
        }
    }
    return (0);
}
