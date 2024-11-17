/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/17 23:06:23 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int     ft_exec(t_cmd_list *list,t_data *data)
{
	pid_t   pid;
	int	status;

	pid = -1;
	if(!list)
		return(data->exit);
	if(!list->next)
		makeapipe(list->pipe);
	if(list->cmd_args[0] && parsebi(list,data) == 0 && !list->next)
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
		if(list->next || data->nodenb != 0)
			ft_execpipe(list, data->nodenb);
		ft_exec1(list);
    		ft_exec2(list, data);
	}
	if(pid > 0)
	{
		// printf("%d \n",data->nodenb);
		// if(list->next)
		// 	close(list->pipe[1]);
		if(data->nodenb != 0)
			close(list->pipe[2]);
	}
	if(list && list->next)
	{
		data->nodenb++;
		list->next->pipe[2] = list->pipe[0];
		ft_exec(list->next,data);
	}
	if(pid > 0 && !list->next)
	{	
		while(waitpid(-1, &status, 0) != pid)
			;
	}
	// if(WIFEXITED(status))
	// 	data->exit = WEXITSTATUS(status);
	return(0);
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
