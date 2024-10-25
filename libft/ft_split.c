/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:10:52 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/18 19:27:21 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s != '\0')
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != '\0' && *s != c)
			s++;
	}
	return (count);
}

static char	*get_next_word(const char **s, char c)
{
	const char	*word_start;
	char		*word;
	size_t		len;
	size_t		i;

	len = 0;
	while (**s == c)
		(*s)++;
	word_start = *s;
	while (**s != '\0' && **s != c)
	{
		len++;
		(*s)++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = word_start[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	size_t	i;
	char	**array;

	if (s == NULL)
		return (NULL);
	words = count_words(s, c);
	array = (char **)malloc(sizeof(char *) * (words + 1));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < words)
	{
		array[i] = get_next_word(&s, c);
		if (array[i] == NULL)
		{
			while (i > 0)
				free(array[--i]);
			free(array);
			return (NULL);
		}
		i++;
	}
	array[words] = NULL;
	return (array);
}
