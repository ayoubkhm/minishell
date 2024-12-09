/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:47:25 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 00:02:27 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>




int write_to_temp_file(int fd, const char *filename, const char *content)
{
    fprintf(stderr, "[DEBUG] Writing to file '%s':\n%s\n", filename, content);
    if (write(fd, content, strlen(content)) == -1)
    {
        perror("Erreur d'écriture dans le fichier temporaire");
        unlink(filename);
        return (-1);
    }
    return (0);
}

char	*strjoin_with_newline(char *s1, char *s2)
{
	size_t	total_len;
	size_t	len1;

	total_len = calculate_total_length(s1, s2);
	char *result = malloc(total_len);
	if (!result)
		return (NULL);
	len1 = 0;
	if (s1)
	{
		len1 = strlen(s1);
		strcpy(result, s1);
	}
	if (s2)
	{
		strcpy(result + len1, s2);
		result[total_len - 2] = '\n';
	}
	result[total_len - 1] = '\0';
	free(s1);
	return (result);
}

size_t	calculate_total_length(char *s1, char *s2)
{
	size_t	len1 = 0;
	size_t	len2 = 0;
	if (s1)
		len1 = strlen(s1);
	if (s2)
		len2 = strlen(s2);
	return (len1 + len2 + 2);
}
