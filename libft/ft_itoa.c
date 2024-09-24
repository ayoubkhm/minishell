/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:35:24 by akhamass          #+#    #+#             */
/*   Updated: 2023/11/19 02:11:09 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	get_num_length(int n)
{
	size_t	length;

	if (n <= 0)
		length = 1;
	else
		length = 0;
	while (n != 0)
	{
		length++;
		n = n / 10;
	}
	return (length);
}

void	itoa_is_negative(int *n, int *negative)
{
	if (*n < 0)
	{
		*n = -*n;
		*negative = 1;
	}
}

char	*ft_itoa(int n)
{
	long	num;
	int		length;
	char	*str;

	num = n;
	length = get_num_length(num);
	str = malloc(sizeof(char) * (length + 1));
	if (str == NULL)
		return (NULL);
	str[length] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	while (num != 0)
	{
		str[--length] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}
