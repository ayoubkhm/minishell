
#include "exec.h"

int	ft_envar(t_token *token, t_data *data)
{
	int		i;

	i = 0;

	token->value = rmstrbfc(token->value, '$');
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], token->value, ft_strlen(token->value)
				- 1) == 0)
		{
			token->value = rmstrbfc(data->envp[i], '=');
			return (0);
		}
		i++;
	}
	printf("%d\n",data->exit);
	if(token->value[0] == '?' && token->value[1] == '\0')
		token->value = ft_itoa(data->exit);
	return(0);
}

// int	ft_envar(t_token *token, t_data *data)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*value;

// 	i = 0;
// 	if (!token || !token->value)
// 		return (1);
// 	tmp = rmstrbfc(token->value, '$');
// 	if (tmp[0] == '?' && !tmp[1])
// 	{
// 		free(token->value);
// 		token->value = ft_itoa(data->exit);
// 		free(tmp);
// 		return (0);
// 	}
// 	while (data->envp[i])
// 	{
// 		if (ft_strncmp(data->envp[i], tmp, ft_strlen(tmp)) == 0
// 			&& data->envp[i][ft_strlen(tmp)] == '=')
// 		{
// 			value = rmstrbfc(data->envp[i], '=');
// 			free(token->value);
// 			token->value = value;
// 			free(tmp);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	free(tmp);
// 	return (0);
// }
