/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:11:07 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:20:22 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

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
