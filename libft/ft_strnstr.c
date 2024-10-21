/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:43:16 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/19 00:47:36 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	len2;

	if (*to_find == '\0')
		return ((char *)str);
	len2 = ft_strlen(to_find);
	while (*str != '\0' && len-- >= len2)
	{
		if (*str == *to_find && ft_strncmp(str, to_find, len2) == 0)
			return ((char *)str);
		str++;
	}
	return (NULL);
}
