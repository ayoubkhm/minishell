/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:43:34 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:43:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	allocate_command_args(t_cmd_list *current_cmd, int arg_count)
{
	int	i;

	i = 0;
	current_cmd->cmd_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!current_cmd->cmd_args)
		return (-1);
	while (i <= arg_count)
	{
		current_cmd->cmd_args[i] = NULL;
		i++;
	}
	return (0);
}

void	remove_empty_arguments(t_cmd_list *current_cmd)
{
	int		arg_count;
	int		new_arg_count;
	char	**new_cmd_args;

	arg_count = 0;
	new_arg_count = 0;
	new_cmd_args = NULL;
	if (!current_cmd || !current_cmd->cmd_args)
		return ;
	new_arg_count = count_non_empty_arguments(current_cmd->cmd_args);
	arg_count = 0;
	while (current_cmd->cmd_args[arg_count])
		arg_count++;
	if (new_arg_count == arg_count)
		return ;
	new_cmd_args = filter_non_empty_arg(current_cmd->cmd_args, new_arg_count);
	if (!new_cmd_args)
		return ;
	free(current_cmd->cmd_args);
	current_cmd->cmd_args = new_cmd_args;
}

int	count_non_empty_arguments(char **cmd_args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (cmd_args && cmd_args[i])
	{
		if (cmd_args[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

char	**allocate_arguments_array(int new_arg_count)
{
	char	**new_cmd_args;

	new_cmd_args = malloc(sizeof(char *) * (new_arg_count + 1));
	if (!new_cmd_args)
		return (NULL);
	new_cmd_args[new_arg_count] = NULL;
	return (new_cmd_args);
}

char	**filter_non_empty_arg(char **cmd_args, int new_arg_count)
{
	char	**new_cmd_args;

	new_cmd_args = NULL;
	new_cmd_args = allocate_arguments_array(new_arg_count);
	if (!new_cmd_args)
		return (NULL);
	populate_arguments_array(cmd_args, new_cmd_args);
	return (new_cmd_args);
}
