/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:36 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/24 19:42:41 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*create_token(char *value, int type, int expand)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->expand = expand; // Correction ici
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!*tokens)
	{
		*tokens = new_token;
	}
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
		new_token->prev = last;
	}
}


char	**append_file(char **files_list, char *file)
{
	int		len;
	char	**new_list;
	int		i;

	len = 0;
	i = 0;
	while (files_list && files_list[len])
		len++;
	new_list = malloc(sizeof(char *) * (len + 2));
	if (!new_list)
		return (NULL);
	while (i < len)
	{
		new_list[i] = files_list[i];
		i++;
	}
	new_list[len] = ft_strdup(file);
	new_list[len + 1] = NULL;
	free(files_list);
	return (new_list);
}

int	*append_file_type(int *files_type, int type)
{
	int	len;
	int	*new_types;
	int	i;

	len = 0;
	i = 0;
	while (files_type && files_type[len])
		len++;
	new_types = malloc(sizeof(int) * (len + 1));
	if (!new_types)
		return (NULL);
	while (i < len)
	{
		new_types[i] = files_type[i];
		i++;
	}
	new_types[len] = type;
	free(files_type);
	return (new_types);
}

int	get_last_index(char **list)
{
	int	len;

	len = 0;
	while (list && list[len])
		len++;
	return (len - 1);
}
