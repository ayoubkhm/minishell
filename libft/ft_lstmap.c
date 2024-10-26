/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:36:32 by gtraiman          #+#    #+#             */
/*   Updated: 2023/11/15 20:04:08 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*dest;
	t_list	*first;
	t_list	*second_elem;

	if (!lst)
		return (NULL);
	dest = ft_lstnew((*f)(lst->content));
	if (dest == NULL)
		return (NULL);
	first = dest;
	lst = lst->next;
	while (lst)
	{
		second_elem = ft_lstnew((*f)(lst->content));
		if (!second_elem)
		{
			ft_lstclear(&first, del);
			return (NULL);
		}
		ft_lstadd_back(&dest, second_elem);
		lst = lst->next;
	}
	return (dest);
}
