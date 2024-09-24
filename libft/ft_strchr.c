/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:12:32 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/19 03:24:25 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	char	ch;

	ch = (char)c;
	while (*str)
	{
		if (*str == ch)
			return ((char *)str);
		++str;
	}
	if (ch == '\0')
		return ((char *)str);
	return (NULL);
}
