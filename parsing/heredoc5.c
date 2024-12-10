/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:13:02 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:13:20 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

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
