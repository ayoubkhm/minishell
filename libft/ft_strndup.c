/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:50:14 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:52:23 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*result;

	result = malloc(n + 1);
	if (!result)
		return (NULL);
	strncpy(result, s, n);
	result[n] = '\0';
	return (result);
}
