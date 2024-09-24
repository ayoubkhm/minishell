/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 20:13:40 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/19 03:39:53 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	is_set(const char *set, char c)
{
	if (!set)
		return (0);
	while (*set != '\0')
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	slen;
	size_t	len;
	char	*s2;
	char	*trim;

	if (s1 == NULL)
		return (NULL);
	slen = ft_strlen(s1);
	s2 = (char *)s1 + slen - 1;
	if (s2 == NULL)
		return (NULL);
	while (*s1 && is_set(set, *(char *)s1))
		s1++;
	while (s2 > s1 && is_set(set, *s2))
		s2--;
	if (s2 >= s1)
		len = s2 - s1 + 1;
	else
		len = 0;
	trim = ft_substr(s1, 0, len);
	return (trim);
}
