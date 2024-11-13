/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/13 20:41:57 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int     ft_exec(t_cmd_list *list,t_data *data)
{
	pid_t   pid;
	int	status;

	if(!list)
		return(data->exit);
	if(list->next)
		makeapipe(list->pipe);
	if(list->cmd_args[0] && parsebi(list,data) == 0)
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
		ft_execpipe(list, data);
		ft_exec1(list);
    		ft_exec2(list, data);
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	if(WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	return(0);
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

int	ft_exec2(t_cmd_list *list, t_data *data)
{
	char	*path;

	path = ft_get_command_path(list->cmd_args[0], data);
	if (!path)
	{
		perror("command not found");
		exit(127);
	}
	if (execve(path, list->cmd_args, data->envp) == -1)
	{
		// perror("execve");
		free(path);
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
