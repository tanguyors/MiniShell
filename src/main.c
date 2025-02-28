
#include "../include/minishell.h"
# include <unistd.h> 

/* A faire : -Intégration de la variable $? permettant de voir la dernière sortie des commandes effectuées.
			 -Tester la robustesse du code, faire un rapport des erreurs. <-- actuellement en cours
			 -Checker les leaks.
			 -Remise à la norme. 
             
             - initialisation de static t_dir_stack *g_dir_stack = NULL; // Pile globale dans le main 
             - faire un pointeur vers la struct s_dir_stack depuis la struct s_shell 
             - utiliser le nouveau pointeur shell->dir_stack->"value" pour acceder aux différente valeur de s_dir_stack */

/* Erreur :  -gérer le std input lors de echo arg | ./minishell  ( isatty )
             -gérer cat < "./test.txt", parsing des quotes?  */
static void ascii_art()
{
    ft_printf("\n\n\n");
    ft_printf("███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓     \n");
    ft_printf("▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n");
    ft_printf("▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n");
    ft_printf("▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n");
    ft_printf("▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n");
    ft_printf("░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n");
    ft_printf("░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n");
    ft_printf("░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n");
    ft_printf("       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n\n\n");
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

static void main_2(struct s_shell shell)
{
    struct s_shell *head;

    signals();
    while (1)
    {
        head = NULL;
        shell.rl_input = readline("minishell> ");
        if (shell.rl_input != NULL)       // Permet d'avoir un historique cmd
            add_history(shell.rl_input);
        if (shell.rl_input == NULL)  // Permet d'exit le shell (ctrl + D)
        {
            rl_clear_history();
            free(shell.rl_input);
            free_list(head);
            exit(shell.exit_code);
        }

        //ft_printf("before parsing exit code: %d\n", shell.exit_code);
        head = parsing(shell.rl_input, head, &shell);
        //ft_printf("after parsing exit code: %d\n", shell.exit_code);
        if (!head)
        {
            if (shell.rl_input)
                free(shell.rl_input);
            main_2(shell);
        }
		parse_execution(&shell, head);
        if (shell.rl_input)
            free(shell.rl_input);
        if (head)
        {
            print_list(head);
			print_token(head);
            free_list(head);
        }
    }
}

int main(void)
{
    struct s_shell shell;
    //ascii_art();
    shell.exit_code = 0;
    main_2(shell);
    return (0);
}
