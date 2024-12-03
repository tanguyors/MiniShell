
#include "../include/minishell.h"
// gestion des signaux

void handle_signal(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGINT)
    {
        printf("> ^C\n");
    }  
    else if (sig == SIGQUIT)
        printf("> ^\\\n");
}