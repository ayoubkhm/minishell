/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 01:04:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/19 06:02:02 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "../minishell.h"

extern int			g_last_exit_status;

int	ft_openout(t_cmd_list *list,char *fname);
int     ft_openin(t_cmd_list *list,char *fname);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_access(char **tab, char *avi, char **path);
char	**ft_get_path(char **envp);
int	ft_strlkforc(char *str,char c);
char	*rmstrbfc(char *str, char c);
void	ft_free_list(t_cmd_list *list);
void	ft_free_inlist(t_cmd_list *list);


int     parsebi(t_cmd_list *list, t_data *data);
int ft_exec(t_cmd_list *list, t_data *data, t_env **env_list);
int	ft_exec1(t_cmd_list *list);
int	ft_exec2(t_cmd_list *list, t_data *data, t_env **env_list);
char	*ft_get_command_path(char *cmd, t_data *data);
int	ft_is_absolute_path(char *cmd);


int	ft_openall(t_cmd_list *list);
int     ft_execpipe(t_cmd_list *list, int n);
int	makeapipe(int *pipefd);
void	ft_close_pipes(int pipefd[2]);
int	ft_execute_child(t_cmd_list *list, t_data *data);
void	initpipe(t_cmd_list *list);

int     ft_heredoc(t_cmd_list *list, t_data *data);


#endif