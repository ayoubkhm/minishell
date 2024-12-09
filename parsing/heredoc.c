#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int	get_heredoc_number(const char *filename, size_t prefix_len)
{
	int	num;
	int	i;

	num = 0;
	i = (int)prefix_len;
	while (filename[i] >= '0' && filename[i] <= '9')
	{
		num = num * 10 + (filename[i] - '0');
		i++;
	}
	return (num);
}

int	find_max_heredoc_number(const t_heredoc_ctx *ctx)
{
	DIR				*dir;
	struct dirent	*entry;
	int				max_num;
	size_t			prefix_len;
	int				num;

	prefix_len = strlen(ctx->suffix);
	max_num = 0;
	dir = opendir(ctx->base_path);
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strncmp(entry->d_name, ctx->suffix, prefix_len) == 0)
		{
			num = get_heredoc_number(entry->d_name, prefix_len);
			if (num > max_num)
				max_num = num;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (max_num);
}

int	count_digits(int n)
{
	int	tmp;
	int	count;

	tmp = n;
	count = 0;
	if (tmp == 0)
		return (1);
	while (tmp > 0)
	{
		tmp /= 10;
		count++;
	}
	return (count);
}

void	write_number(char *buf, int n, int digit_count)
{
	int	tmp;
	int	i;

	tmp = n;
	i = digit_count - 1;
	while (tmp > 0)
	{
		buf[i] = (tmp % 10) + '0';
		tmp /= 10;
		i--;
	}
}

void	int_to_str(int n, char *buf, size_t buf_size)
{
	int	tmp;
	int	digit_count;

	tmp = n;
	if (tmp == 0)
	{
		if (buf_size > 1)
		{
			buf[0] = '0';
			buf[1] = '\0';
		}
		return ;
	}
	digit_count = count_digits(tmp);
	if ((size_t)digit_count + 1 > buf_size)
	{
		buf[0] = '\0';
		return ;
	}
	buf[digit_count] = '\0';
	write_number(buf, n, digit_count);
}

char	*allocate_filename(t_heredoc_ctx *ctx)
{
	char	*new_filename;
	char	number_buf[32];
	size_t	total_length;

	ctx->counter++;
	while (1)
	{
		int_to_str(ctx->counter, number_buf, sizeof(number_buf));
		total_length = strlen(ctx->base_path) + strlen(ctx->suffix);
		total_length += strlen(number_buf) + 1;
		new_filename = malloc(total_length);
		if (!new_filename)
			return (NULL);
		new_filename[0] = '\0';
		strcat(new_filename, ctx->base_path);
		strcat(new_filename, ctx->suffix);
		strcat(new_filename, number_buf);
		if (access(new_filename, F_OK) != 0)
			return (new_filename);
		free(new_filename);
		new_filename = NULL;
		ctx->counter++;
	}
}

char	*get_base_filename(void)
{
	char	*base_filename;

	base_filename = calloc(4096, sizeof(char));
	if (!base_filename)
		return (NULL);
	if (!getcwd(base_filename, 4096))
	{
		free(base_filename);
		return (NULL);
	}
	return (base_filename);
}

int	init_ctx(t_heredoc_ctx *ctx, const char *base_filename)
{
	ctx->base_path = strdup(base_filename);
	if (!ctx->base_path)
		return (0);
	ctx->counter = find_max_heredoc_number(ctx) + 1;
	return (1);
}

int	crecri_file(char *f_name, char *cntnt, char *base_path, char *base_f_name)
{
	int	fd;

	fd = open(f_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free((char *)base_path);
		free(base_f_name);
		return (0);
	}
	if (write(fd, cntnt, strlen(cntnt)) == -1)
	{
		unlink(f_name);
		close(fd);
		free((char *)base_path);
		free(base_f_name);
		return (0);
	}
	close(fd);
	free((char *)base_path);
	free(base_f_name);
	return (1);
}

void	init_heredoc_ctx(t_heredoc_ctx *ctx)
{
	ctx->counter = 0;
	ctx->base_path = NULL;
	ctx->suffix = "/heredoc_tmp_";
}

char	*handle_init_error(char *base_filename)
{
	free(base_filename);
	return (NULL);
}

char	*gere_filename_error(const t_heredoc_ctx *ctx, char *base_filename)
{
	free((char *)ctx->base_path);
	free(base_filename);
	return (NULL);
}

char	*handle_write_error(char *filename)
{
	free(filename);
	return (NULL);
}

char	*create_temp_file(char *content)
{
	t_heredoc_ctx	ctx;
	char			*base_filename;
	char			*filename;

	init_heredoc_ctx(&ctx);
	base_filename = get_base_filename();
	if (!base_filename)
		return (NULL);
	if (!init_ctx(&ctx, base_filename))
		return (handle_init_error(base_filename));
	filename = allocate_filename(&ctx);
	if (!filename)
		return (gere_filename_error(&ctx, base_filename));
	if (!crecri_file(filename, content, ctx.base_path, base_filename))
		return (handle_write_error(filename));
	return (filename);
}

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
