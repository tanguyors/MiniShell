
#include "../include/minishell.h"
// gestion des signaux

void handle_signal(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 1);
        rl_redisplay();
    }  
    else if (sig == SIGQUIT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 1);
        rl_redisplay();
    }
}
