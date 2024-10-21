/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:47:49 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/19 03:25:33 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static size_t	ft_len(unsigned int start, size_t len, size_t slen)
{
	if (len > slen - start)
		len = slen - start;
	return (len);
}

static char	*ft_nullmall(char *dst)
{
	dst = malloc(1);
	if (!dst)
		return (NULL);
	dst[0] = '\0';
	return (dst);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	slen;
	char	*dst;

	i = 0;
	dst = NULL;
	if (s == NULL)
		return (ft_nullmall((char *)s));
	slen = ft_strlen(s);
	if (start >= slen || len == 0)
		return (ft_nullmall(dst));
	len = ft_len(start, len, slen);
	dst = malloc(sizeof(char) * (len + 1));
	if (!dst)
		return (NULL);
	while (i < len && s[start + i] != '\0')
	{
		dst[i] = s[start + i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
