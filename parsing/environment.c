/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:09:47 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/08 14:36:59 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	populate_arguments_array(char **cmd_args, char **new_cmd_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd_args[i])
	{
		if (cmd_args[i][0] != '\0')
		{
			new_cmd_args[j] = cmd_args[i];
			j++;
		}
		else
		{
			free(cmd_args[i]);
		}
		i++;
	}
}

void	set_env_variable(t_env **env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*new_var;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = strdup(value);
			return ;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_env));
	new_var->name = strdup(name);
	new_var->value = strdup(value);
	new_var->next = *env_list;
	*env_list = new_var;
}

char	*get_env_variable(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

void	add_env_variable(char *arg, t_env **env_list)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	size_t	name_len;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		name_len = equal_sign - arg;
		name = ft_strndup(arg, name_len);
		value = ft_strdup(equal_sign + 1);
		set_env_variable(env_list, name, value);
		free(name);
		free(value);
	}
}

void	handle_export(t_cmd_list *cmd, t_env **env_list)
{
	int		i;
	char	*arg;

	i = 1;
	while (cmd->cmd_args[i])
	{
		arg = cmd->cmd_args[i];
		add_env_variable(arg, env_list);
		i++;
	}
}
