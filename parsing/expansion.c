#include "parsing.h"

char	*extract_variable_naming(char *arg, int *i)
{
	int		var_start;
	char	*var_name;

	var_start = *i;
	while (arg[*i] && (isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
	{
		*i = *i + 1;
	}
	var_name = ft_strndup(&arg[var_start], *i - var_start);
	return (var_name);
}

char	*append_variable_value2(char *result, char *var_name, t_env *env_list)
{
	char	*var_value;
	char	*temp;

	var_value = get_env_variable(env_list, var_name);
	if (var_value)
	{
		temp = result;
		result = ft_strjoin(result, var_value);
		free(temp);
	}
	return (result);
}

char	*expand_variable(char *arg, int *i, t_env *env_list, char *result)
{
	char	*var_name;

	*i = *i + 1;
	if (!arg[*i] || !(isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
	{
		result = append_character_main(result, '$');
		return (result);
	}
	var_name = extract_variable_naming(arg, i);
	if (!var_name)
		return (result);
	result = append_variable_value2(result, var_name, env_list);
	free(var_name);
	return (result);
}

char	*append_character_main(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = result;
	result = ft_strjoin(result, temp_str);
	free(temp);
	return (result);
}

char	*handle_normal_character(char *result, char *arg, int *i)
{
	result = append_character_main(result, arg[(*i)++]);
	return (result);
}

char	*handle_dollar(char *result, char *arg, int *i, t_env *env_list)
{
	if (arg[*i + 1] == '?')
	{
		(*i)++;
	}
	else if (!arg[*i + 1] || !(isalnum((unsigned char)arg[*i + 1]) || arg[*i + 1] == '_'))
	{
		result = append_character_main(result, arg[(*i)++]);
	}
	else
	{
		result = expand_variable(arg, i, env_list, result);
	}
	return (result);
}

char	*expand_variables(char *arg, t_env *env_list)
{
	char	*result;
	int		i;

	result = strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
			result = handle_dollar(result, arg, &i, env_list);
		else
			result = handle_normal_character(result, arg, &i);
	}
	return (result);
}
