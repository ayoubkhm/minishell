/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:11:29 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:12:21 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

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
