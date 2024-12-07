/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:47:25 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 01:47:26 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	create_unique_temp_file(const char *base_filename, char *filename)
{
	int	fd;
	int	suffix;

	fd = -1;
	suffix = 0;
	while (1)
	{
		sprintf(filename, "%s%d", base_filename, suffix);
		fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			break ;
		if (errno != EEXIST)
		{
			perror("Erreur de création du fichier temporaire");
			return (-1);
		}
		suffix++;
	}
	return (fd);
}

char	*allocate_filename(const char *base_filename)
{
	char	*filename;

	filename = malloc(strlen(base_filename) + 10);
	if (!filename)
		perror("Erreur d'allocation de mémoire pour le nom du fichier");
	return (filename);
}

int	write_to_temp_file(int fd, const char *filename, const char *content)
{
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
	char	*result;
	size_t	total_len;
	size_t	len1;

	total_len = calculate_total_length(s1, s2);
	result = malloc(total_len);
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
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = strlen(s1);
	if (s2)
		len2 = strlen(s2);
	return (len1 + len2 + 2);
}
