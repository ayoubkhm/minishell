/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/09/23 20:26:06 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 20:26:06 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "parsing/parsing.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_pwd(char **envp);
int	ft_env(char **envp);
char	*ft_addstr(char *str1, char *str2);
int	ft_unset(char **envp, char *str);
void	ft_delstr(char *str);
char	**ft_copytab(char **tab);
int	ft_tabstrlen(char **tab);
char	**ft_export(char **envp, char *str);

#endif