/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 01:04:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/07 22:26:06 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "../minishell.h"

extern int	g_last_exit_status;

int			ft_openout(t_cmd_list *list, char *fname);
int			ft_openin(t_cmd_list *list, char *fname);
int			fdinout(t_cmd_list *list, int *fd_in, int *fd_out, int i);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_access(char **tab, char *avi, char **path);
int			ft_access2(char **path, char *str);
char		**ft_get_path(char **envp, t_data *data, t_cmd_list *list);
int			ft_strlkforc(char *str, char c);
char		*rmstrbfc(char *str, char c);
void		ft_free_list(t_cmd_list *list);
void		ft_free_inlist(t_cmd_list *list);

int			checkbi(t_cmd_list *list);
int			parsebi(t_cmd_list *list, t_data *data, t_env **env_list);
int			ft_exec(t_cmd_list *list, t_data *data, t_env **env_list);
void		ft_execinchild(t_cmd_list *list, t_data *data, t_env **env_list);

int			ft_exec1(t_cmd_list *list, t_data *data, t_env **env_list);
int			ft_exec1par(t_cmd_list *list, t_data *data);
int			ft_exec2(t_cmd_list *list, t_data *data, t_env **env_list);
char		*ft_get_command_path(char *cmd, t_data *data, t_cmd_list *list);
int			ft_is_absolute_path(char *cmd);
int			ft_iscinstr(char *str, char c);
int			ft_weirdparse(t_cmd_list *list);
char		**ft_envi(void);

int			ft_openall(t_cmd_list *list);
int			ft_execpipe(t_cmd_list *list);
int			makeapipe(int *pipefd);
void		ft_close_pipes(int pipefd[2]);
int			ft_execute_child(t_cmd_list *list, t_data *data, t_env **env_list);
void		initpipe(t_cmd_list *list);

char		**ft_extract_path(char **envp, t_data *data);
char		*ft_get_current_dir(void);
char		**ft_updateshlvl(char **tab);
char		*ft_atoishlvl(char *str);
void		ifshlvl(char *tab, char **tabret, int i);
char		*ft_copystrfromn(char *str, int n);
char		**ft_updateshlvl(char **tab);
int			ft_heredoc(t_cmd_list *list, char *str);
void		ft_waitall(t_data *data, int lpid);
int			ft_exechild(t_cmd_list *list, t_data *data, t_env **env_list);
int			ft_resbi(t_cmd_list *list, t_data *data, t_env **env_list, int i);

#endif