/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 19:29:51 by akhamass          #+#    #+#             */
/*   Updated: 2024/04/22 19:32:19 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_type(char pourcent_type, va_list args)
{
	if (pourcent_type == '%')
		return (write(1, "%", 1));
	else if (pourcent_type == 'c')
	{
		ft_putchar_fd(va_arg(args, int), 1);
		return (1);
	}
	else if (pourcent_type == 's')
		return (prt_str(va_arg(args, char *)));
	else if (pourcent_type == 'p')
		return (prt_ptr(va_arg(args, void *)));
	else if (pourcent_type == 'd' || pourcent_type == 'i')
		return (prt_int(va_arg(args, int)));
	else if (pourcent_type == 'u')
		return (prt_unsigned(va_arg(args, unsigned int)));
	else if (pourcent_type == 'x' || pourcent_type == 'X')
		return (prt_hexa(va_arg(args, ssize_t), (pourcent_type == 'X')));
	return (0);
}

int	ft_printf(const char *__format, ...)
{
	int		i;
	int		len;
	va_list	args;

	if (!__format)
		return (0);
	i = 0;
	len = 0;
	va_start(args, __format);
	while (__format[i])
	{
		if (__format[i] == '%')
		{
			i++;
			len += check_type(__format[i], args);
		}
		else
			len += write(1, &__format[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
