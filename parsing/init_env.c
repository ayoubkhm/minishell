/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:17:26 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 05:49:17 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	extract_name_value(char *env_var, char **name, char **value)
{
	char	*equal_sign;
	size_t	name_len;

	if (!env_var || !name || !value)
		return ;
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
	if (!*name || !*value)
	{
		free(*name);
		free(*value);
		*name = NULL;
		*value = NULL;
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
	new_var->exit_status = 0;
	return (new_var);
}

t_env	*init_default_env(void)
{
	t_env	*env_list;
	t_env	*current;

	env_list = create_env_node("PWD", "/");
	current = env_list;
	current->next = create_env_node("SHLVL", "1");
	current = current->next;
	current->next = create_env_node("_", "/usr/bin/env");
	return (env_list);
}

t_env	*add_env_node(t_env *env_list, char *name, char *value)
{
	t_env	*current;

	if (!env_list)
		return (create_env_node(name, value));
	current = env_list;
	while (current->next)
		current = current->next;
	current->next = create_env_node(name, value);
	return (env_list);
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	char	*name;
	char	*value;

	env_list = NULL;
	if (!envp || !*envp)
		return (init_default_env());
	while (*envp)
	{
		extract_name_value(*envp, &name, &value);
		env_list = add_env_node(env_list, name, value);
		envp++;
	}
	return (env_list);
}
