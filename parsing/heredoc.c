#include "parsing.h"

char	*allocate_temp_filename(char *base_filename)
{
	char	*filename;

	filename = malloc(strlen(base_filename) + 10);
	if (!filename)
		perror("Error during allocation");
	return (filename);
}

int	find_available_filename(char *filename, char *base_filename)
{
	int	fd;
	int	suffix;

	suffix = 0;
	while (1)
	{
		sprintf(filename, "%s%d", base_filename, suffix);
		fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			return (fd);
		if (errno != EEXIST)
		{
			perror("Erreur de création du fichier temporaire");
			return (-1);
		}
		suffix++;
	}
}

int	write_to_temp_file(int fd, char *filename, char *content)
{
	if (write(fd, content, strlen(content)) == -1)
	{
		perror("Erreur d'écriture dans le fichier temporaire");
		close(fd);
		unlink(filename);
		return (-1);
	}
	return (0);
}

char	*create_temp_file(char *content)
{
	char	*base_filename;
	char	*filename;
	int		fd;

	base_filename = "/tmp/heredoc";
	filename = allocate_temp_filename(base_filename);
	if (!filename)
		return (NULL);
	fd = find_available_filename(filename, base_filename);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	if (write_to_temp_file(fd, filename, content) == -1)
	{
		free(filename);
		return (NULL);
	}
	close(fd);
	return (filename);
}

char	*allocate_result_with_newline(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	total_len;

	if (s1)
		len1 = ft_strlen(s1);
	else
		len1 = 0;
	if (s2)
		len2 = ft_strlen(s2);
	else
		len2 = 0;
	total_len = len1 + len2 + 2;
	return (malloc(total_len));
}

char	*strjoin_with_newline(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (s1)
		len1 = ft_strlen(s1);
	else
		len1 = 0;
	if (s2)
		len2 = ft_strlen(s2);
	else
		len2 = 0;
	result = allocate_result_with_newline(s1, s2);
	if (!result)
		return (NULL);
	if (s1)
		ft_strcpy(result, s1);
	if (s2)
	{
		ft_strcpy(result + len1, s2);
		result[len1 + len2] = '\n';
	}
	result[len1 + len2 + 1] = '\0';
	free(s1);
	return (result);
}

char	*get_heredoc(char *delimiter)
{
	char	*content;
	char	*line;

	content = NULL;
	line = readline(NULL);
	while (line)
	{
		if (strcmp(line, delimiter) == 0)
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

char	*get_heredoc_delimiter(t_token *current)
{
	char	*delimiter;

	delimiter = ft_strdup(current->next->value);
	if (!current->next || current->next->type != TYPE_WORD)
	{
		fprintf(stderr, "Syntax error: missing heredoc delimiter\n");
		return (NULL);
	}
	if (!delimiter)
	{
		fprintf(stderr, "Error: failed to allocate heredoc delimiter\n");
	}
	return (delimiter);
}

char	*create_temp_file_with_content(char *content)
{
	char	*filename;

	filename = create_temp_file(content);
	if (!content)
	{
		content = strdup("");
		if (!content)
		{
			fprintf(stderr, "Error: failed to allocate memory\n");
			return (NULL);
		}
	}
	if (!filename)
	{
		fprintf(stderr, "Error: failed to create heredoc temp file\n");
	}
	return (filename);
}

char	**realloc_file_list(char **file_list, size_t file_cnt, char *temp_file)
{
	size_t	old_size;
	size_t	new_size;
	char	**new_files_list;

	old_size = sizeof(char *) * (file_cnt + 1);
	new_size = sizeof(char *) * (file_cnt + 2);
	new_files_list = ft_realloc(file_list, old_size, new_size);
	if (!new_files_list)
		return (NULL);
	new_files_list[file_cnt] = temp_file;
	new_files_list[file_cnt + 1] = NULL;
	return (new_files_list);
}

int	*realloc_files_type(int *files_type, size_t files_count, int file_type)
{
	size_t	old_size;
	size_t	new_size;
	int		*new_files_type;

	old_size = sizeof(int) * files_count;
	new_size = sizeof(int) * (files_count + 1);
	new_files_type = ft_realloc(files_type, old_size, new_size);
	if (!new_files_type)
		return (NULL);
	new_files_type[files_count] = file_type;
	return (new_files_type);
}

int	add_temp_file_to_cmd(t_cmd_list *curr_cmd, char *temp_filename)
{
	char	**new_files_list;
	int		*new_files_type;

	new_files_list = realloc_file_list(curr_cmd->files_list,
			curr_cmd->files_count, temp_filename);
	if (!new_files_list)
	{
		free(temp_filename);
		return (-1);
	}
	new_files_type = realloc_files_type(curr_cmd->files_type,
			curr_cmd->files_count, TYPE_HEREDOC);
	if (!new_files_type)
	{
		free(temp_filename);
		free(new_files_list);
		return (-1);
	}
	curr_cmd->files_list = new_files_list;
	curr_cmd->files_type = new_files_type;
	curr_cmd->files_count++;
	return (0);
}

int	handle_single_heredoc(t_token *current, t_cmd_list *curr_cmd)
{
	char	*heredoc_delimiter;
	char	*heredoc_content;
	char	*temp_filename;

	heredoc_delimiter = get_heredoc_delimiter(current);
	if (!heredoc_delimiter)
		return (-1);
	heredoc_content = get_heredoc(heredoc_delimiter);
	free(heredoc_delimiter);
	if (!heredoc_content)
		return (-1);
	temp_filename = create_temp_file_with_content(heredoc_content);
	free(heredoc_content);
	if (!temp_filename)
		return (-1);
	if (add_temp_file_to_cmd(curr_cmd, temp_filename) == -1)
	{
		free(temp_filename);
		return (-1);
	}
	return (0);
}

int	process_heredoc(t_token **tokens, t_cmd_list *curr_cmd)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type == TYPE_HEREDOC)
	{
		if (handle_single_heredoc(current, curr_cmd) == -1)
			return (-1);
		current = current->next->next;
	}
	*tokens = current;
	return (0);
}
