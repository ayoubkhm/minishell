/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:09:47 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/24 19:13:52 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	set_env_variable(t_env **env_list, char *name, char *value)
{
	t_env	*current;
	t_env	*new_var;

	current = *env_list;
	while (current)
	{
		if (strcmp(current->name, name) == 0)
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

char *get_env_variable(t_env *env_list, char *name)
{
    t_env *current = env_list;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return ft_strdup(current->value); // Retourne une copie pour éviter les problèmes de double `free`.
        }
        current = current->next;
    }
    return ft_strdup(""); // Retourne une chaîne vide si la variable n'existe pas.
}

void	add_env_variable(char *arg, t_env **env_list)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	size_t	name_len;

	equal_sign = strchr(arg, '=');
	if (equal_sign)
	{
		name_len = equal_sign - arg;
		name = strndup(arg, name_len);
		value = strdup(equal_sign + 1);
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
