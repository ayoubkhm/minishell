/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:17:26 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 13:19:03 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	extract_name_value(char *env_var, char **name, char **value)
{
	char	*equal_sign;
	size_t	name_len;

	equal_sign = strchr(env_var, '=');
	if (equal_sign)
	{
		name_len = equal_sign - env_var;
		*name = ft_strndup(env_var, name_len);
		*value = strdup(equal_sign + 1);
	}
	else
	{
		*name = ft_strdup(env_var);
		*value = ft_strdup("");
	}
}

t_env	*create_env_node(char *name, char *value)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->name = name;
	new_var->value = value;
	new_var->next = NULL;
	return (new_var);
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*current;
	char	*name;
	char	*value;

	env_list = NULL;
	current = NULL;
	while (*envp)
	{
		extract_name_value(*envp, &name, &value);
		if (!env_list)
		{
			env_list = create_env_node(name, value);
			current = env_list;
		}
		else
		{
			current->next = create_env_node(name, value);
			current = current->next;
		}
		envp++;
	}
	return (env_list);
}
