
// gestion de l'executions des commande
#include "../include/minishell.h"

/* Remise a la norme de l'initialisation de ta structure t_builtin */
static void initialize_builtin(t_builtin *builtin)
{
    builtin[0] = (t_builtin){"pwd", ft_pwd};
    builtin[1] = (t_builtin){"echo", ft_echo};
    builtin[2] = (t_builtin){"env", ft_env};
    builtin[3] = (t_builtin){"export", ft_export};
    builtin[4] = (t_builtin){"unset", ft_unset};
    builtin[5] = (t_builtin){"cd", ft_cd};
    builtin[6] = (t_builtin){"exit", ft_exit};
	builtin[7] = (t_builtin){NULL, NULL};
}
/* implémenter strtok, strncpy et strcat */
char *get_absolute_path(char *command)
{
    static char path[1024];
	char path_copy[1024];
    char *path_env;
    char *dir;

    // Copier PATH pour éviter de modifier l'original
	path_env = getenv("PATH");
    if (path_env == NULL) 
	{
        perror("Error PATH not found\n");
        return (NULL);
    }

    strncpy(path_copy, path_env, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';
    // Parcourir chaque répertoire dans PATH
	// Utilisation de strtok pour clear le PATH
    dir = strtok(path_copy, ":");
    while (dir != NULL) 
	{
        // Construire le chemin à partir de 'dir' et 'command'
        ft_strcpy(path, dir);
        strcat(path, "/");
        strcat(path, command);

        if (access(path, X_OK) == 0) 
            return (path);

        dir = strtok(NULL, ":");
    }
    return (NULL);
}

void std_execution(struct s_shell *current)
{
	pid_t pid;
	char *command;
	char **args;

    pid = fork();
    if (pid == -1) 
	{
        perror("Erreur lors de fork");
        exit(EXIT_FAILURE);
    } 
	else if (pid == 0) 
	{
        command = get_absolute_path(current->data);
        args = get_all_data(current);
        char *env[] = { NULL };

        if (execve(command, args, env) == -1) 
		{
            perror("Erreur lors de l'exécution de la commande");
			free_array(args);
            exit(EXIT_FAILURE);
        }
		free_array(args);
    } 
	else 
        waitpid(pid, NULL, 0);
}

/* Initialise les builtin et parcourt l'array builtin afin de trouver la commande correspondante
	si la commande reste introuver cela signifie qu'il s'agit d'une commande système
	std_execution() sera donc appelé */
void cmd_execution(struct s_shell *current, char **data)
{
	t_builtin builtin[8];
	int i;

	i = 0;
	printf("CMD_EXECUTION !\n");
	initialize_builtin(builtin);
    // Parcourt la table des commandes internes
    while (builtin[i].name != NULL)
    {
		//printf("current data: %s\n", current->data);
        if (ft_strcmp(current->data, builtin[i].name) == 0)
        {
            builtin[i].func(data); // Appelle la fonction correspondante.
            return;
        }
        i++;
    }
	// Si aucune commande builtin ne correspond
	std_execution(current);
    //ft_printf("minishell: %s: command not found\n", current->data);
}

/* Data des arguments des commandes uniquement 
	mémoire alloué au char ** */
char **get_arg_data(struct s_shell *current)
{
	int i;
	struct s_shell *p_arg;
	char **data;

	data = malloc(sizeof(char *) * 1024);
	if (!data)
		exit_with_error("allocation error");
	i = 0;
	p_arg = current;
	//printf("p_arg: %s\n", p_arg->data);
	while (p_arg)
	{
		if (p_arg->token == TOKEN_ARG || p_arg->token == TOKEN_SIMPLE_QUOTE || p_arg->token == TOKEN_DOUBLE_QUOTE)
		{
			data[i] = p_arg->data;
			printf("data[%d]: %s\n", i, data[i]);
			i++;
		}
		if (p_arg->token == TOKEN_PIPE)
			break;
		p_arg = p_arg->next;
	}
	return (data);
}

/* Data des commandes + leurs arguments 
	mémoire alloué au char ** */
char **get_all_data(struct s_shell *current)
{
    int i;
	struct s_shell *p_arg;
	char **data;

	data = malloc(sizeof(char *) * 1024);
	if (!data)
		exit_with_error("allocation error");
	i = 0;
	p_arg = current;
	//printf("p_arg: %s\n", p_arg->data);
	while (p_arg)
	{
		if (p_arg->token == TOKEN_CMD || p_arg->token == TOKEN_ARG)
		{
			data[i] = p_arg->data;
			printf("data[%d]: %s\n", i, data[i]);
			i++;
		}
		if (p_arg->token == TOKEN_PIPE)
			break;
		p_arg = p_arg->next;
	}
	return (data);
}

/* Version adapté de ta fonction parse_command() pour la liste chaînée 
	Lis la commande étant dans le token_cmd et la compare avec le tableau builtin 
	si la commande correspond celle ci est ensuite executé
	Pour chaque argument dans token_arg ils sont stocké dans data qui est ensuite,
	donné comme char ** aux fonctions builtin */

/* J'ai adapté l'éxecution de telle sorte a ne pas modifié tes fonctions builtin
	si ça te parrais mieux de les modifier tu peux faire comme ça et changer également cette fonction */

/* cmd_execution() passe des tests classiques mais il y aura certainement des changement a faire par la suite
	dans celle ci mais aussi les builtin */
void extract_data(struct s_shell *current)
{
	char **data;

	printf("EXTRACT_DATA !\n");
	data = get_arg_data(current);
	cmd_execution(current, data);
	free(data);
}

/**
 * Gestion de la redirection d'entrée (<)
 * Redirige l'entrée standard vers un fichier
 */
static void redir_input(struct s_shell *current)
{
    int fd;
	int saved_stdin;

    if (!current || !current->next || current->next->token != TOKEN_FILE)
        return;
    
    current = current->next; // Déplacement vers le TOKEN_FILE
    fd = open(current->data, O_RDONLY);
    if (fd == -1)
    {
        printf("minishell: %s: No such file or directory\n", current->data);
        return;
    }
    // Sauvegarde de l'entrée standard originale
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        close(fd);
        return;
    }
    // Redirection de l'entrée standard
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        close(saved_stdin);
        return;
    }
    // Exécution de la commande si présente
    //if (current->next && current->next->token == TOKEN_CMD)
        //extract_data(current->next);
    // Restauration de l'entrée standard
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(fd);
}

/**
 * Gestion de la redirection de sortie (>)
 * Redirige la sortie standard vers un fichier
 */
static void redir_output(struct s_shell *current)
{
    int fd;
	int saved_stdout;
	struct s_shell *head;

	head = current;
	printf("OUTPUT\n");
	print_list(head);
    if (!current || !current->next)
        return;

    current = current->next->next;
    fd = open(current->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("minishell: %s: Permission denied\n", current->data);
        return;
    }
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return;
    }
	//printf("redir_output current token: %s\n", get_token_name(current->token));
    if (head && head->token == TOKEN_CMD)
        extract_data(head);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

/**
 * Gestion de la redirection en mode append (>>)
 * Ajoute la sortie à la fin du fichier
 */
static void redir_append(struct s_shell *current)
{
    int fd;
	int saved_stdout;

    if (!current || !current->next || current->next->token != TOKEN_FILE)
        return;

    current = current->next;
    fd = open(current->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_printf("minishell: %s: Permission denied\n", current->data);
        return;
    }
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return;
    }
    if (current->next && current->next->token == TOKEN_CMD)
        extract_data(current->next);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

/**
 * Gestion du heredoc (<<)
 * Lit l'entrée jusqu'à ce que le délimiteur soit rencontré
 */
static void redir_heredoc(struct s_shell *current)
{
    int pipefd[2];
    char *line;
    size_t len;
	int saved_stdin;

    if (!current || !current->next || current->next->token != TOKEN_FILE)
        return;

    current = current->next;
    if (pipe(pipefd) == -1)
        return;

    while (1)
    {
        printf("> ");
        line = get_next_line(STDIN_FILENO);
        if (!line)
            break;

        // Suppression du newline à la fin
        len = strlen(line);
        if (len > 0 && line[len-1] == '\n')
            line[len-1] = '\0';

        // Vérification du délimiteur
        if (strcmp(line, current->data) == 0)
        {
            free(line);
            break;
        }

        // Écriture dans le pipe
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    // Redirection de l'entrée standard vers le pipe
    saved_stdin = dup(STDIN_FILENO);
    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) != -1)
    {
        if (current->next && current->next->token == TOKEN_CMD)
            extract_data(current->next);
    }
    // Restauration
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(pipefd[0]);
}


/* Redirige vers le bon token de redirection */
void redirection_execution(struct s_shell *current)
{
	printf("REDIRECTION !\n");
	print_list(current);
	if (current->next->token == REDIR_INPUT)
	{
		redir_input(current);
	}
	else if (current->next->token == REDIR_OUTPUT)
	{
		redir_output(current);
	}
	else if (current->next->token == REDIR_APPEND)
	{
		redir_append(current);
	}
	else if (current->next->token == REDIR_HEREDOC)
	{
		redir_heredoc(current);
	}
}

/* Devenu obsolète après l'ajout de multi_pipe_handling() */
void pipe_handling(struct s_shell **current_pipe, struct s_shell *current)
{
	int fd[2];
	int pid1;
	int pid2;

	if (pipe(fd) == -1)
		exit_with_error("pipe error");
	pid1 = fork();
	if (pid1 < 0)
		exit_with_error("fork error");
	if (pid1 == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			exit_with_error("dup2 error fd[1]");
		close(fd[0]);
		close(fd[1]);
		extract_data(*current_pipe);
		exit(EXIT_SUCCESS);
	}
	//printf("current_pipe data: %s\n", (*current_pipe)->data);
	(*current_pipe) = (*current_pipe)->next;
	while ((*current_pipe) && (*current_pipe)->token != TOKEN_CMD)
		(*current_pipe) = (*current_pipe)->next;
	//printf("current_pipe data: %s\n", (*current_pipe)->data);
	//printf("current: %s\n", get_token_name((*current_pipe)->token));
	pid2 = fork();
	if (pid2 < 0)
	exit_with_error("fork error");
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) < 0)
			exit_with_error("dup2 error fd[0]");
		close(fd[0]);
		close(fd[1]);
		extract_data(*current_pipe);
		exit(EXIT_SUCCESS);
	}
	close(fd[0]);
	close(fd[1]);
	if (waitpid(pid1, NULL, 0) == -1)
		exit_with_error("waitpid error");
	if (waitpid(pid2, NULL, 0) == -1)
		exit_with_error("waitpid error");
}

static void child_process(int fd[2], int prev_fd, struct s_shell *current)
{
	int nb_pipe;
	struct s_shell *current_redir;

	current_redir = current;
	nb_pipe = is_pipe(current);
	// Si un pipe précédent existe, connectez-le à STDIN
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
			exit_with_error("dup2 error prev_fd");
		close(prev_fd);
	}
	if (current_redir->next)
	{
		while(!is_token_red(current_redir->next->token)) // ajouter is_token_red(current->next->next->token)
		{
			current_redir = current_redir->next;
		}
		if (is_token_red(current_redir->token))
			redirection_execution(current_redir);
	}
	// Si un pipe suivant existe, connectez-le à STDOUT
	printf("test current token: %s\n", get_token_name(current->token));
	//printf("nb_pipe: %d\n", nb_pipe);
	if (nb_pipe)
	{
		nb_pipe--;
		printf("PASS\n");
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			exit_with_error("dup2 error fd[1]");
	}
	close(fd[0]);
	close(fd[1]);
	extract_data(current);
	exit(EXIT_SUCCESS);
}

static void pipe_and_fork(int fd[2], int *pid)
{
	if (pipe(fd) == -1)
		exit_with_error("pipe error");

	*pid = fork();
	if (*pid < 0)
		exit_with_error("fork error");
}

/* Permet de gérer le cas où un pipe est présent dans la liste. 
	Utilisation de fork afin de créer un processus enfant, 
	celui ci va redirigé la sortie de la commande en fonction des pipes. 
	Ici le double pointeur current représente la liste chaînée complète*/
void multi_pipe_handling(struct s_shell *current)
{
    int fd[2];
    int prev_fd;
    pid_t pid;

	prev_fd = -1;
    while (current)
    {
		printf("MULTI_PIPE_HANDLING !\n");
        pipe_and_fork(fd, &pid);
        if (pid == 0)
			child_process(fd, prev_fd, current);
        // Parent : Gérer les descripteurs
        if (prev_fd != -1)
            close(prev_fd);
 		if (current->next)
        {
            close(fd[1]);    // Fermer le côté écriture du pipe actuel
            prev_fd = fd[0]; // Garder le côté lecture pour la prochaine commande
        }
        else
            close(fd[0]); // Pas de commande suivante, fermer les descripteurs restants
		
        current = current->next;
        while (current && current->token != TOKEN_CMD)
		{
			//redirection_execution(*current);
			current = current->next;	
		}
		
    }
    while (wait(NULL) > 0)
        continue;
}


void exec_without_pipe(struct s_shell *current)
{
	while (current)
	{
		if (is_redirection_in_list(current))
		{
			if (current->next->token)
			{
				if (is_token_red(current->next->token)) // relié a TOKEN_RED, cherche REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
				{
					redirection_execution(current);
				}
			}
			current = current->next;
		}
		else
		{
			if (current->token)
			{
				if (current->token == TOKEN_CMD) // relié a TOKEN_ARG, cherche un token ARG
				{
					extract_data(current);
				}
			}
			current = current->next;
		}
	}
}

/* Permet de trier les executions des commandes,
	en parcourant les tokens de la liste chaînée */
/* Passage par référence necessaire ? */
void parse_execution(struct s_shell *head)
{
	struct s_shell *current;

	current = head;
	if (!is_pipe(current))
	{
		exec_without_pipe(current);
	}
	else
	{
		multi_pipe_handling(current);
	}
}
