/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/25 21:56:20 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int ft_exec(t_cmd_list *list, t_data *data, t_env **env_list)
{
	pid_t   pid;
	int	status;

	if(!list)
		return(data->exit);
	if(list->next)
		makeapipe(list->pipe);
    if (list->cmd_args[0] && parsebi(list, data, env_list) == 0 && !list->next)
			return(0);
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		return(-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if(list->next || list->prev)
			ft_execpipe(list);
		ft_exec1(list, data, env_list);
    		ft_exec2(list, data, env_list);
	}
	if(pid > 0)
	{
		if(list->prev)
			close(list->prev->pipe[0]);
		if(list->next)	
		{
			close(list->pipe[1]);
			ft_exec(list->next,data,env_list);
		}
		else
		{
			while (waitpid(-1, &status, 0) > 0)
			{
				if (WIFEXITED(status))
					data->exit = WEXITSTATUS(status);
			}
		}
	}
	return(0);
}

void	initpipe(t_cmd_list *list)
{
	while(list)
	{
		list->pipe[0] = 0;
		list->pipe[1] = 0;
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
		return (ft_strdup(cmd));
	tab = ft_get_path(data->envp);
	if(!tab)
		return(ft_freetab(tab),NULL);
	if (ft_access(tab, cmd, &path) == -1)
		return (ft_freetab(tab),NULL);
	ft_freetab(tab);
	return (path);
}


int ft_exec2(t_cmd_list *list, t_data *data, t_env **env_list)
{
	char	*path;

	path = ft_get_command_path(list->cmd_args[0], data);
	if (!path)
	{
		free(path);
        	cleanup_resources(data, env_list, list);
		g_last_exit_status = 127;
		exit(g_last_exit_status);
	}
	if (execve(path, list->cmd_args, data->envp) == -1)
	{
		// perror("execve");
		free(path);
        cleanup_resources(data, env_list, list);
		g_last_exit_status = 1;
		exit(g_last_exit_status);
	}
	return (0);
}

int parsebi(t_cmd_list *list, t_data *data, t_env **env_list)
{
        if(ft_strcmp(list->cmd_args[0],"cd") == 0)
                return(ft_cd(list,data));
        if(ft_strcmp(list->cmd_args[0],"echo") == 0)
                return(ft_parsecho(list, data));
        if(ft_strcmp(list->cmd_args[0],"env") == 0)
                return(ft_env(data->envp));
        if(ft_strcmp(list->cmd_args[0],"exit") == 0)
	        ft_exit(data, env_list, list);
        if(ft_strcmp(list->cmd_args[0],"pwd") == 0)
                return(ft_pwd(data));
        if(ft_strcmp(list->cmd_args[0],"unset") == 0)
                return(ft_parsunset(data,list));
        if(ft_strcmp(list->cmd_args[0],"export") == 0)
                return(ft_parsexport(data,list));
        return(1);
}
