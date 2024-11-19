# include "../minishell.h"

int ft_exec(t_cmd_list *list, t_data *data, t_env **env_list)
{
    pid_t pid;
    int status;

    if (!list)
        return (data->exit);

    if (list->next)
    {
        if (pipe(list->pipe) == -1)
        {
            perror("pipe");
            cleanup_resources(data, env_list, list);
            exit(1);
        }
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        if (data->nodenb != 0)
        {
            dup2(data->prev_pipe_read_end, STDIN_FILENO);
            close(data->prev_pipe_read_end);
        }

        if (list->next)
        {
            close(list->pipe[0]);
            dup2(list->pipe[1], STDOUT_FILENO);
            close(list->pipe[1]);
        }

        ft_exec1(list);
        ft_exec2(list, data, env_list);
        cleanup_resources(data, env_list, list);
        exit(0);
    }
    else
    {
        // Parent process

        // Close previous read end in parent
        if (data->nodenb != 0)
            close(data->prev_pipe_read_end);

        if (list->next)
        {
            close(list->pipe[1]); // Close write end in parent
            data->prev_pipe_read_end = list->pipe[0]; // Save read end for next command
        }

        // Wait for the child process
        waitpid(pid, &status, 0);
        // Handle the status if necessary

        // Recursive call for the next command
        if (list->next)
        {
            data->nodenb++;
            ft_exec(list->next, data, env_list);
        }
        else
        {
            // If this is the last command, reset nodenb and prev_pipe_read_end
            data->nodenb = 0;
            data->prev_pipe_read_end = -1;
        }
    }

    return (0);
}


void	initpipe(t_cmd_list *list)
{
	while(list)
	{
		list->pipe[0] = 0;
		list->pipe[1] = 0;
		list->pipe[2] = 0;
		list = list->next;
	}
}

int	ft_is_absolute_path(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (0);
	return (cmd[0] == '/');
}

char	*ft_get_command_path(char *cmd, t_data *data)
{
	char	*path;
	char	**tab;

	path = NULL;
	if (ft_is_absolute_path(cmd))
		return (cmd);
	tab = ft_get_path(data->envp);
	if (ft_access(tab, cmd, &path) == -1)
		return (ft_freetab(tab),NULL);
	return (path);
}

int	ft_exec2(t_cmd_list *list, t_data *data, t_env **env_list)
{
	char	*path;

	path = ft_get_command_path(list->cmd_args[0], data);
	if (!path)
	{
		perror("command not found");
        cleanup_resources(data, env_list, list);
		exit(127);
	}
	if (execve(path, list->cmd_args, data->envp) == -1)
	{
		// perror("execve");
		free(path);
        cleanup_resources(data, env_list, list);

		exit(1);
	}
	return (0);
}

int     parsebi(t_cmd_list *list, t_data *data)
{
        if(ft_strcmp(list->cmd_args[0],"cd") == 0)
                return(ft_cd(list,data));
        if(ft_strcmp(list->cmd_args[0],"echo") == 0)
                return(ft_parsecho(list, data));
        if(ft_strcmp(list->cmd_args[0],"env") == 0)
                return(ft_env(data->envp));
        if(ft_strcmp(list->cmd_args[0],"exit") == 0)
                ft_exit(data,list);
        if(ft_strcmp(list->cmd_args[0],"pwd") == 0)
                return(ft_pwd(data));
        if(ft_strcmp(list->cmd_args[0],"unset") == 0)
                return(ft_parsunset(data,list));
        if(ft_strcmp(list->cmd_args[0],"export") == 0)
                return(ft_parsexport(data,list));
        return(1);
}
