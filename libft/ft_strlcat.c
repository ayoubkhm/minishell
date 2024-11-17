/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:29:55 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/18 19:29:58 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	i = dest_len;
	if (size <= dest_len)
	{
		return (size + src_len);
	}
	while (i < size - 1 && src[i - dest_len] != '\0')
	{
		dest[i] = src[i - dest_len];
		i++;
	}
	if (i < size)
		dest[i] = '\0';
	return (dest_len + src_len);
}
