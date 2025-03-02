#include "../include/minishell.h"

int ft_echo(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int i = 1;
    int newline = 1;
    char *result;
    i = 1;
    while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    // Afficher directement argv[i], déjà expansé
    while (argv[i] && !is_spec_char(argv[i][0]))
    {
        printf("argv[%d]: %s\n", i, argv[i]);
        ft_putstr_fd(argv[i], 1);
        //if (argv[i + 1])
            //ft_putchar_fd(' ', 1);
        i++;
    }

    if (newline)
        ft_putchar_fd('\n', 1);

    shell->exit_code = 0;
    return (0);
}