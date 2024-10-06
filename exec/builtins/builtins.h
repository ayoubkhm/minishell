

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../../minishell.h"


int	ft_echo(char *str);
bool	is_n_option(char *arg);
int	ft_parsecho(t_token *token);
int	ft_env(char **envp);
char	**ft_export(char **envp, char *str);
int	ft_parsexport(t_data *data,t_token *token);
int	ft_envexport(char **envp);
int	ft_pwd(char **envp);
char	**ft_unset(char **envp, char *str);
int	ft_parsunset(t_data *data,t_token *token);

// UTILS
char	*ft_addstr(char *str1, char *str2);
void	ft_freedata(t_data *data);
void	ft_freetab(char **tab);
char	**ft_copytab(char **tab);
int	ft_tabstrlen(char **tab);






#endif