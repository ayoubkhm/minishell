/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/03 20:29:57 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int     ft_exec(t_token *token,t_data *data)
{
        pid_t   pid;
        
        if(parsebi(token,data) == 0)
                return(0);
        pid = fork();
        if(pid == -1)
        {
                perror("fork");
                return(-1);
        }
        if (pid == 0)
        {

                if(ft_exec2(token,data) == -1)
                        exit(1);
        }
        return(0);
}

int     ft_exec2(t_token *token,t_data *data)
{
        char **path;
        
        path = NULL;
        token->tab = ft_get_path(data->envp);
	if (!token->tab)
		return (ft_freetab(token->tab), -1);
        if(ft_access(token->tab,token->value,path) == -1)
                exit(127);
                
        return(0);
}



int     parsebi(t_token *token, t_data *data)
{
        if(ft_strcmp(token->value,"echo") == 0)
                return(ft_parsecho(token));
        if(ft_strcmp(token->value,"env") == 0)
                return(ft_env(data->envp));
        if(ft_strcmp(token->value,"pwd") == 0)
                return(ft_pwd(data->envp));
        if(ft_strcmp(token->value,"unset") == 0)
                return(ft_parsunset(data,token));
        if(ft_strcmp(token->value,"export") == 0)
                return(ft_parsexport(data,token));
        return(1);
}
