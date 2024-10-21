/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 20:59:35 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int     ft_exec(t_token *token,t_data *data)
{
	pid_t   pid;
	int	status;

	if(!token)
		return(data->exit);
	if(token->type == 0 || token->type == 7)
	{
		if(parsebi(token,data) == 0)
			return(0);
	}
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
			return(-1);
	}
	if (pid == 0)
		ft_checkarg(token, data);
	else if(pid > 0)
		waitpid(pid, &status, 0);
	if(WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	return(0);
}

char	**ft_tokentab(t_token *token)
{
	int	i;
	char	**tab;
	
	tab = malloc((tklast(token) + 1) * sizeof(char *));
	if (!tab)
		return(NULL);
	i = 0;
	while(token && (token->type == 0 || token->type == 7))
	{
		tab[i] = token->value;
		token = token->next;
		i++;
	}
	tab[i] = NULL;
	return(tab);
}


// int     ft_exec2(t_token *token, t_data *data)
// {
//         char	*path;
// 	char	**tab;

//         path = NULL;
// 	tab = ft_tokentab(token);
// 	if (!tab)
// 	{
// 		perror("token");
// 		exit(1);
// 	}
// 	if(tab[0][0] == '/')
// 		path = tab[0];
// 	else
// 	{	
// 		if (ft_access(ft_get_path(data->envp), token->value, &path) == -1)
// 			exit(127);
// 	}
//         if (token->next && (token->next->type == 0 || token->next->type == 7))
//                 token = token->next;
// 	printf("%s\n", tab[0]);
//         if (execve(path, tab, data->envp) == -1)
// 		perror("execve");
//         return (0);
// }

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
	//if(tab)
	//	ft_freetab(tab);
	return (path);
}

int	ft_exec2(t_token *token, t_data *data)
{
	char	*path;
	char	**tab;

	tab = ft_tokentab(token);
	if (!tab)
	{
		perror("token");
		exit(1);
	}
	path = ft_get_command_path(tab[0], data);
	if (!path)
	{
		perror("command not found");
		exit(127);
	}
	// if(token->next)
	// 	ft_execflw(token, data);
	if (execve(path, tab, data->envp) == -1)
	{
		perror("execve");
		ft_freetab(tab);
		free(path);
		exit(1);
	}
	return (0);
}


int	ft_checkarg(t_token *token, t_data *data)
{
	while(token)
	{
		// printf("%s\n",token->value);
		if(token->type == 0)
			ft_exec2(token, data);
		else if(token->type == 1)
			ft_rdrin(token, data);
		else if(token->type == 2)
			ft_rdrout(token, data);
		else if(token->type == 3)
			ft_rdrapp(token, data);
		else if(token->type == 4)
			ft_heredoc(token, data);
		// else if(token->type == 5)
		// 	ft_envar(token, data);
		else if(token->type == 6)
			ft_execpipe(token, data);
		else if(token->type == 7)
			{}//TODO "blabla"
		token = token->next;
	}
	return(0);
}




int     parsebi(t_token *token, t_data *data)
{
        if(ft_strcmp(token->value,"cd") == 0)
                return(ft_cd(token,data));
        if(ft_strcmp(token->value,"echo") == 0)
                return(ft_parsecho(token, data));
        if(ft_strcmp(token->value,"env") == 0)
                return(ft_env(data->envp));
        if(ft_strcmp(token->value,"exit") == 0)
                ft_exit(data,token);
        if(ft_strcmp(token->value,"pwd") == 0)
                return(ft_pwd(data->envp));
        if(ft_strcmp(token->value,"unset") == 0)
                return(ft_parsunset(data,token));
        if(ft_strcmp(token->value,"export") == 0)
                return(ft_parsexport(data,token));
        return(1);
}
