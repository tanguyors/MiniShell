
#include "../include/minishell.h"

int main(void)
{
    struct s_shell value;
    struct s_shell *head;
    struct sigaction sa;
    char **tokens;
    
    sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    while (1)
    {
        int i;
        char *input;
        
        head = NULL;
        i = 0;
        input = readline("> ");
        if (input != NULL)       // Permet d'avoir un historique
            add_history(input);
        else if (input == NULL)  // Permet d'exit le shell (ctrl + D)
            exit(EXIT_SUCCESS);
            
        //tokens = parse_tokens(input);   //  <-- a décommenter pour tes tests
        
        /* temporary exit */
        //if(tokens && ft_strcmp(tokens[0], "exit") == 0) //  <-- a décommenter pour tes tests
            //exit(EXIT_SUCCESS);

        /*while(tokens && tokens[i])  //  <-- a décommenter pour tes tests
        {
            printf("token [%d]: %s\n", i, tokens[i]);
            i++;
        }*/
        //while(i--)   // <-- a décommenter pour tes tests
            //insert_head(&head, tokens[i]);
        //parse_commands(tokens);
        head = parsing(input, head);
        //ft_echo(tokens);
        //ft_pwd();
        if (head)
        {
            print_list(head);
            free_list(head);
        }
    }
    return (0);
}
