/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:38:15 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/18 18:38:21 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int value, size_t num)
{
	const unsigned char	*str;

	str = ptr;
	while (num--)
	{
		if (*str == (unsigned char)value)
			return ((void *)str);
		str++;
	}
	return (NULL);
}
