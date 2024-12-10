/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:09:42 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:20:36 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
