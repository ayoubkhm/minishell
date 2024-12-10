/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:10:03 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:20:18 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

char	*get_heredoc(char *delimiter)
{
	char	*content;
	char	*line;

	content = NULL;
	line = NULL;
	printf("> ");
	line = readline(NULL);
	while (line)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = strjoin_with_newline(content, line);
		free(line);
		printf("> ");
		line = readline(NULL);
	}
	return (content);
}

int	update_cmd_files(t_cmd_list *curr_cmd, char *temp_filename)
{
	char	**new_files_list;
	int		*new_files_type;
	size_t	new_files_list_size;
	size_t	new_files_type_size;

	new_files_list_size = sizeof(char *) * (curr_cmd->files_count + 2);
	new_files_type_size = sizeof(int) * (curr_cmd->files_count + 1);
	new_files_list = ft_realloc(curr_cmd->files_list, new_files_list_size);
	new_files_type = ft_realloc(curr_cmd->files_type, new_files_type_size);
	if (!new_files_list || !new_files_type)
	{
		return (-1);
	}
	curr_cmd->files_list = new_files_list;
	curr_cmd->files_type = new_files_type;
	curr_cmd->files_list[curr_cmd->files_count] = temp_filename;
	curr_cmd->files_type[curr_cmd->files_count] = TYPE_HEREDOC;
	curr_cmd->files_list[curr_cmd->files_count + 1] = NULL;
	curr_cmd->files_count++;
	return (0);
}

int	handle_heredoc(t_token *current, t_cmd_list *curr_cmd, char **temp_filename)
{
	char	*heredoc_delimiter;
	char	*heredoc_content;

	heredoc_delimiter = ft_strdup(current->next->value);
	if (!heredoc_delimiter)
		return (-1);
	heredoc_content = get_heredoc(heredoc_delimiter);
	free(heredoc_delimiter);
	if (!heredoc_content)
		return (-1);
	*temp_filename = create_temp_file(heredoc_content);
	free(heredoc_content);
	if (!*temp_filename)
		return (-1);
	if (update_cmd_files(curr_cmd, *temp_filename) == -1)
	{
		free(*temp_filename);
		*temp_filename = NULL;
		return (-1);
	}
	return (0);
}

int	process_heredoc(t_token **tokens, t_cmd_list *curr_cmd)
{
	t_token	*current;
	char	*temp_filename;

	current = *tokens;
	temp_filename = NULL;
	while (current && current->type == TYPE_HEREDOC)
	{
		if (!current->next || current->next->type != TYPE_WORD)
			return (-1);
		if (handle_heredoc(current, curr_cmd, &temp_filename) == -1)
			return (-1);
		current = current->next->next;
	}
	*tokens = current;
	return (0);
}
