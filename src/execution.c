
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
/* implémenter strtok, strcpy et strcat */
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
		ft_putstr_fd("Error PATH not found\n", 2);
        return (NULL);
    }
    ft_strncpy(path_copy, path_env, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';
    // Parcourir chaque répertoire dans PATH
	// Utilisation de strtok pour clear le PATH
    dir = ft_strtok(path_copy, ":");
    while (dir != NULL) 
	{
        // Construire le chemin à partir de 'dir' et 'command'
        ft_strcpy(path, dir);
        ft_strcat(path, "/");
        ft_strcat(path, command);
        if (access(path, X_OK) == 0) 
            return (path);

        dir = ft_strtok(NULL, ":");
    }
    return (NULL);
}

void std_execution(struct s_shell *shell, struct s_shell *current)
{
	pid_t pid;
	char *command;
	char **args;
	extern char **environ;
	int status; // Variable pour stocker le code de sortie

    pid = fork();
    if (pid == -1) 
		exit_with_error("fork error", NULL, 1);
	else if (pid == 0) 
	{
        command = get_absolute_path(current->data);
		if (!command)
			command = current->data; // cas de ./
        args = get_all_data(current);
        if (execve(command, args, environ) == -1) 
		{
			if(ft_strchr(current->data, '/') && ft_strchr(current->data, '.'))
			{
				perror(" execve: ");
				exit_with_error(NULL, args, 126);
			}
			else
			{
				ft_putstr_fd(" command not found\n", 2);
				exit_with_error(NULL, args, 127);
			}
        }
		free_array(args);
    }
    else
    {
        waitpid(pid, &status, 0); // Attendre le processus enfant
        if (WIFEXITED(status)) // Vérifier si le processus a terminé normalement
            shell->exit_code = WEXITSTATUS(status); // Mettre à jour le code de sortie
		else if (WIFSIGNALED(status))
			shell->exit_code = WTERMSIG(status) + 128;
    }
	//ft_printf("std execution exit code: %d\n", shell->exit_code);
}

/* Initialise les builtin et parcourt l'array builtin afin de trouver la commande correspondante
	si la commande reste introuver cela signifie qu'il s'agit d'une commande système
	std_execution() sera donc appelé */
void cmd_execution(struct s_shell *shell, struct s_shell *current, char **data)
{
	t_builtin builtin[8];
	int i;

	i = 0;
	initialize_builtin(builtin);
    // Parcourt la table des commandes internes
    while (builtin[i].name != NULL)
    {
		//printf("current data: %s\n", current->data);
        if (ft_strcmp(current->data, builtin[i].name) == 0)
        {
            builtin[i].func(data, shell); // Appelle la fonction correspondante.
            return;
        }
        i++;
    }
	//ft_printf("cmd execution exit code: %d\n", shell->exit_code);
	// Si aucune commande builtin ne correspond
	std_execution(shell, current);
    //ft_printf("minishell: %s: command not found\n", current->data);
}

/* Data des arguments des commandes uniquement 
	mémoire alloué au char ** */
char **get_arg_data(struct s_shell *current)
{
	int i;
	struct s_shell *p_arg;
	char **data;

	data = ft_calloc(1024, sizeof(char *));
	if (!data)
		exit_with_error("allocation error", NULL, 1);
	i = 0;
	p_arg = current;
	//printf("p_arg: %s\n", p_arg->data);
	while (p_arg)
	{
		if (p_arg->token == TOKEN_ARG || p_arg->token == TOKEN_SIMPLE_QUOTE || p_arg->token == TOKEN_DOUBLE_QUOTE)
		{
			data[i] = p_arg->data;
			//printf("get arg data: %s\n", data[i]);
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

	data = ft_calloc(1024, sizeof(char *));
	if (!data)
		exit_with_error("allocation error", NULL, 1);
	i = 0;
	p_arg = current;
	//printf("p_arg: %s\n", p_arg->data);
	while (p_arg)
	{
		if (p_arg->token == TOKEN_CMD || p_arg->token == TOKEN_ARG)
		{
			data[i] = p_arg->data;
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
void extract_data(struct s_shell *shell, struct s_shell *current)
{
	char **data;

	//ft_printf("extract data exit code: %d\n", shell->exit_code);
	data = get_arg_data(current);
	cmd_execution(shell, current, data);
	free(data);
	//ft_printf("after extract data exit code: %d\n", shell->exit_code);
}

static int setup_redirection(struct s_shell *shell, struct s_shell *current, int flag, int file_access)
{
	int fd;

	if (!shell->file || !shell->file->next)
        return (-1);
	
    while (shell->file && shell->file->token != TOKEN_FILE)
	{
		//printf("setup [%d]\n", i);
		shell->file = shell->file->next;					 // Déplacement vers le TOKEN_FILE
		//ft_printf("test data: %s\n", shell->file->data);
	}
	fd = open(shell->file->data, flag, file_access);
	if (fd == -1)
	{
		//ft_putstr_fd(" No such file or directory\n", 2);
		perror("open: ");
		shell->exit_code = 1;
		return (-1);
	}
	shell->file = shell->file->next;
	return(fd);
}

/* Gestion de la redirection d'entrée (<)
	Redirige l'entrée standard vers un fichier 
	resultat < file affiché sur la console même avant un pipe ( a regler ) */
static void redir_input(struct s_shell *shell, struct s_shell *current)
{
    int fd;
	int saved_stdin;
	struct s_shell *head;

	head = current;
	fd = setup_redirection(shell, current, O_RDONLY, 0);
	if (fd == -1)
	{
		//ft_putstr_fd(" No such file or directory\n", 2);
		perror("open: ");
		shell->exit_code = 1;
		return ;
	}
	//if (fd == -1)
		//return ;
    // Sauvegarde de l'entrée standard originale
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        close(fd);
        return ;
    }
    // Redirection de l'entrée standard
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        close(saved_stdin);
        return ;
    }
    // Exécution de la commande si présente
    if (head && head->token == TOKEN_CMD  && !shell->file)
        extract_data(shell, head);
    // Restauration de l'entrée standard
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(fd);
}

/* Gestion de la redirection de sortie (>)
	Redirige la sortie standard vers un fichier */
static void redir_output(struct s_shell *shell, struct s_shell *first_arg)
{
    int fd;
	int saved_stdout;
	struct s_shell *head;

	head = first_arg;
	fd = setup_redirection(shell, first_arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return ;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return ;
    }
	//printf("redir_output current token: %s\n", get_token_name(current->token));
    if (head && head->token == TOKEN_CMD && !shell->file)
        extract_data(shell, head);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

/* Gestion de la redirection en mode append (>>)
	Ajoute la sortie à la fin du fichier */
static void redir_append(struct s_shell *shell, struct s_shell *current)
{
    int fd;
	int saved_stdout;
	struct s_shell *head;

	head = current;

	fd = setup_redirection(shell, current, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return ;
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return ;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return ;
    }
    if (head && head->token == TOKEN_CMD && !shell->file)
        extract_data(shell, head);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

void setup_heredoc(struct s_shell **current, int (*pipe_fd)[2])
{
	if (!*current || !(*current)->next)
        return ;
	while (*current && (*current)->next && (*current)->token != TOKEN_FILE)
		*current = (*current)->next;
    if (pipe(*pipe_fd) == -1)
        return ;
}

/* Permet de gérer le cas ou une variable d'environnement
	doit être interprêté */
char *h_expand_var(char *line, char **new_line, char **end_line)
{
	char *expanded_var;
	char *dollar_pos;
	char *before_dollar;
	char *after_dollar;

	dollar_pos = ft_strchr(line, '$');
	if (dollar_pos)
	{
		expanded_var = expand_variable(dollar_pos + 1);
		//ft_printf("test newline: %s\n", new_line);
		before_dollar = ft_substr(line, 0, dollar_pos - line);
		*new_line = ft_strjoin(before_dollar, expanded_var);
		free(before_dollar);
		while (*dollar_pos != ' ' && *dollar_pos)
			dollar_pos++;
		after_dollar = ft_substr(line, dollar_pos - line, expanded_var - line);
		*end_line = ft_strjoin(*new_line, after_dollar);
		free(line);
		free(after_dollar);
		line = *end_line;
		//free(end_line);
		free(*new_line);
	}
	return (line);
}

void loop_heredoc(struct s_shell *current, int (*pipe_fd)[2])
{
	char *line;
	char *new_line;
	char *end_line;
	size_t len;

	//printf("loop heredoc: %s\n", current->data);
    while (1)
    {
		write(1, "heredoc> ", 9);
        line = get_next_line(STDIN_FILENO);
		if (!line)
            break;
		line = h_expand_var(line, &new_line, &end_line);
        // Suppression du newline à la fin
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        // Vérification du délimiteur
        if (ft_strcmp(line, current->data) == 0)
        {
            free(line);
            break;
        }
        // Écriture dans le pipe
        write(pipe_fd[0][1], line, ft_strlen(line));
        write(pipe_fd[0][1], "\n", 1);
        free(line);
    }
}

/* Gestion du heredoc (<<)
	Lit l'entrée jusqu'à ce que le délimiteur soit rencontré */
static void redir_heredoc(struct s_shell *shell, struct s_shell *current)
{
    int pipe_fd[2];
	int saved_stdin;
	struct s_shell *head;
	
	head = current;
	setup_heredoc(&current, &pipe_fd);
	if (*pipe_fd == -1)
		return ;
	loop_heredoc(current, &pipe_fd);
    // Redirection de l'entrée standard vers le pipe
    saved_stdin = dup(STDIN_FILENO);
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) != -1)
    {
        if (head && head->token == TOKEN_CMD && !shell->file)
            extract_data(shell, head);
    }
    // Restauration
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(pipe_fd[0]);
}


/* Redirige vers le bon token de redirection 
	SUPPR PRINTF erreur heredoc */
void redirection_execution(struct s_shell *shell, struct s_shell *first_arg)
{
	struct s_shell *which_redir;

	which_redir = first_arg;
	while (!is_token_red(which_redir->token))
		which_redir = which_redir->next;
	if (which_redir->token == REDIR_INPUT)
	{
		redir_input(shell, first_arg);
	}
	else if (which_redir->token == REDIR_OUTPUT)
	{
		redir_output(shell, first_arg);
	}
	else if (which_redir->token == REDIR_APPEND)
	{
		redir_append(shell, first_arg);
	}
	else if (which_redir->token == REDIR_HEREDOC)
	{
		redir_heredoc(shell, first_arg);
	}
}

/* Devenu obsolète après l'ajout de multi_pipe_handling() */
/*void pipe_handling(struct s_shell **current_pipe, struct s_shell *current)
{
	int fd[2];
	int pid1;
	int pid2;

	if (pipe(fd) == -1)
		exit_with_error("pipe error", NULL);
	pid1 = fork();
	if (pid1 < 0)
		exit_with_error("fork error", NULL);
	if (pid1 == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			exit_with_error("dup2 error fd[1]", NULL);
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
	exit_with_error("fork error", NULL);
	if (pid2 == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) < 0)
			exit_with_error("dup2 error fd[0]", NULL);
		close(fd[0]);
		close(fd[1]);
		extract_data(*current_pipe);
		exit(EXIT_SUCCESS);
	}
	close(fd[0]);
	close(fd[1]);
	if (waitpid(pid1, NULL, 0) == -1)
		exit_with_error("waitpid error", NULL);
	if (waitpid(pid2, NULL, 0) == -1)
		exit_with_error("waitpid error", NULL);
}*/

/*static void child_redir_old(struct s_shell *shell, struct s_shell *current)
{
	struct s_shell *current_redir;
	struct s_shell *first_arg;
	int flag;

	flag = 0;
	current_redir = current;
	if (current_redir->next)
	{
		while(current_redir->next && !is_token_red(current_redir->token)) // ajouter is_token_red(current->next->next->token)
		{
			if (current_redir->next->token == TOKEN_ARG && !flag)
			{
				first_arg = current_redir;
				flag = 1;
			}
			current_redir = current_redir->next;
		}
		if (is_token_red(current_redir->token))
		{
			redirection_execution(shell, current_redir);
		}
	}	
}*/

static void child_redir(struct s_shell *shell, struct s_shell *current)
{
	struct s_shell *current_redir;
	struct s_shell *first_arg;

	shell->file = current;
	current_redir = current;
	current_redir = current_redir->next;
	while (current)
	{
		if (is_redirection_in_list(current))
		{
			if (current_redir->next)
			{
				while(current_redir && current_redir->token != TOKEN_CMD) // ajouter is_token_red(current->next->next->token)
				{
					current_redir = current_redir->next;
				}
				redirection_execution(shell, current_redir);
				current = current->next;
			}
		}
		current = current->next;
	}
	//ft_printf("child redir exit code: %d\n", shell->exit_code);
}

static void child_process(struct s_shell *shell, int fd[2], int prev_fd, struct s_shell *current)
{
	int nb_pipe;

	nb_pipe = is_pipe(current);
	// Si un pipe précédent existe, connectez-le à STDIN
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
			exit_with_error("dup2 error prev_fd", NULL, 1);
		close(prev_fd);
	}
	//ft_printf("child process exit code: %d\n", shell->exit_code);
	child_redir(shell, current);
	//ft_printf("TEST EXIT PIPE: %d\n", shell->exit_code);
	// Si un pipe suivant existe, connectez-le à STDOUT
	//printf("test current token: %s\n", get_token_name(current->token));
	//printf("nb_pipe: %d\n", nb_pipe);
	if (nb_pipe)
	{
		nb_pipe--;
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			exit_with_error("dup2 error fd[1]", NULL, 1);
	}
	close(fd[0]);
	close(fd[1]);
	extract_data(shell, current);
	//ft_printf("child process exit code 2: %d\n", shell->exit_code);
	exit(shell->exit_code);
}

static void pipe_and_fork(int fd[2], int *pid)
{
	if (pipe(fd) == -1)
		exit_with_error("pipe error", NULL, 1);

	*pid = fork();
	if (*pid < 0)
		exit_with_error("fork error", NULL, 1);
}

static void m_p_h_wait(struct s_shell *shell, int last_pid, int pid, int status)
{
	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	//else if (WIFSIGNALED(status))
		//shell->exit_code = WTERMSIG(status) + 128;
}

static void m_p_h_fork_suceed(struct s_shell *current, int prev_fd, int fd[2])
{
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next)
	{
		close(fd[1]);
		prev_fd = fd[0];
	}
	else
		close(fd[0]);
}

/* Permet de gérer first_arg cas où un pipe est présent dans la liste. 
	Utilisation de fork afin de créer un processus enfant, 
	celui ci va redirigé la sortie de la commande en fonction des pipes. 
	Ici le double pointeur current représente la liste chaînée complète*/
void multi_pipe_handling(struct s_shell *shell, struct s_shell *current)
{
    int fd[2];
    int prev_fd;
    pid_t pid;
    pid_t last_pid;
    int status;

	last_pid = -1;
    prev_fd = -1;
    while (current)
    {
        pipe_and_fork(fd, &pid);
        if (pid == 0)
            child_process(shell, fd, prev_fd, current);
        else
        {
            last_pid = pid;  // Mettre à jour le dernier PID à chaque fork
			//m_p_h_fork_suceed(current, prev_fd, fd);
			if (prev_fd != -1)
				close(prev_fd);
			if (current->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
			else
				close(fd[0]);
        }
        current = current->next;
        while (current && current->token != TOKEN_CMD)
            current = current->next;
    }
	 // Attendre le dernier processus enfant et récupérer son statut
	if (last_pid != -1)
		m_p_h_wait(shell, last_pid, pid, status);
	while (wait(NULL) > 0)
        continue;
    //ft_printf("multi pipe exit code: %d\n", shell->exit_code);
}


void exec_without_pipe(struct s_shell *shell, struct s_shell *head)
{
	int flag;
	struct s_shell *first_arg;

	flag = 0;
	first_arg = head;
	shell->file = head;
	while (head)
	{
		if (is_redirection_in_list(head))
		{
			if (head->next->token == TOKEN_ARG && !flag)
			{
				first_arg = head;
				flag = 1;
			}
			if (head->token || head->next->token )
			{
				if (is_token_red(head->next->token) || is_token_red(head->token)) // relié a TOKEN_RED, cherche REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
				{
					redirection_execution(shell, first_arg);
					head = head->next;
				}
			}
		}
		else
		{
			if (head && head->token)
			{
				if (head->token == TOKEN_CMD) // relié a TOKEN_ARG, cherche un token ARG
				{
					extract_data(shell, head);
				}
			}
		}
		head = head->next;
	}
}

/* Permet de trier les executions des commandes,
	en parcourant les tokens de la liste chaînée */
/* Passage par référence necessaire ? */
void parse_execution(struct s_shell *shell, struct s_shell *head)
{
	if (!is_pipe(head))
	{
		//ft_printf("before parse exit code: %d\n", shell->exit_code);
		exec_without_pipe(shell, head);
		//ft_printf("after parse exit code: %d\n", shell->exit_code);
	}
	else
	{
		multi_pipe_handling(shell, head);
	}
}
