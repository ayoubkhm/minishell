/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 01:07:02 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/30 01:13:40 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = malloc(new_size);
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!new_ptr)
	{
		fprintf(stderr, "Error: failed to allocate memory in ft_realloc\n");
		return (NULL);
	}
	if (ptr)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}
