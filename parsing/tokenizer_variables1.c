#include "parsing.h"

int ft_str_is_digit(const char *str)
{
    int i = 0;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int ft_str_is_alnum_underscore(const char *str)
{
    int i = 0;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

char *get_numeric_var_value(t_ctx *ctx, const char *var_name)
{
	(void)ctx;
	(void)var_name;
    return ft_strdup("");
}

char *expand_mult_variable_name(t_ctx *ctx, const char *var_name)
{
    char *value;

    if (ft_strcmp(var_name, "?") == 0)
    {
        value = ft_itoa(ctx->e_l->exit_status);
        return value; 
    }
    else if (ft_str_is_digit(var_name))
    {
        value = get_numeric_var_value(ctx, var_name);
        return value;
    }
    else if (ft_str_is_alnum_underscore(var_name))
    {
        // On tente de récupérer la valeur depuis l'environnent
        // Assure-toi que getenv est accessible ou que tu as un équivalent.
        // Sinon, inspire-toi de gere_valid_var pour récupérer la valeur. 
        value = getenv(var_name);
        if (value)
            return ft_strdup(value);
        else
            return ft_strdup("");
    }
    else
    {
        return ft_strdup("");
    }
}


char *expand_variable_name(t_ctx *ctx, const char *var_name)
{
    char *value;
    char *env_val;

    if (strcmp(var_name, "?") == 0)
    {
        value = ft_itoa(ctx->e_l->exit_status);
        return value;
    }
    else if (ft_str_is_digit(var_name))
    {
        value = get_numeric_var_value(ctx, var_name);
        if (!value)
        {
            value = ft_strdup("");
        }
        return value;
    }
    else if (ft_str_is_alnum_underscore(var_name))
    {
        env_val = get_env_variable(ctx->e_l, (char *)var_name);
        if (env_val)
            value = ft_strdup(env_val);
        else
            value = ft_strdup("");
        return value;
    }
    else
    {
        value = ft_strdup("");
        return value;
    }
}





int handle_multiple_values(t_ctx *ctx)
{
    int start = ctx->i;
    int end;
    char *segment;
    char **variables;
    char *final_expanded_value;
    char *expanded_var;
    char *tmp;
    int j;

    end = start;
    while (ctx->inp[end] && !isspace(ctx->inp[end]) && !is_operator(ctx->inp[end]))
        end++;
    segment = ft_substr(ctx->inp, start, end - start);
    if (!segment)
    {
        return (-1);
    }
    variables = ft_split(segment, '$');
    free(segment);
    if (!variables)
    {
        return (-1);
    }
    j = 0;
    while (variables[j])
    {
        j++;
    }
    final_expanded_value = ft_strdup("");
    if (!final_expanded_value)
    {
        free_split_values(variables);
        return (-1);
    }
    j = 0;
    while (variables[j])
    {
        if (variables[j][0] != '\0')
        {
            expanded_var = expand_variable_name(ctx, variables[j]);
            if (!expanded_var)
            {
                free(final_expanded_value);
                free_split_values(variables);
                return (-1);
            }
            tmp = ft_strjoin(final_expanded_value, expanded_var);
            free(final_expanded_value);
            free(expanded_var);

            if (!tmp)
            {
                free_split_values(variables);
                return (-1);
            }
            final_expanded_value = tmp;
        }
        j++;
    }
    free_split_values(variables);
    add_token(ctx->tok, create_token(final_expanded_value, TYPE_WORD, 0));
    free(final_expanded_value);
    if (ctx->dol_seq)
    {
        free(ctx->dol_seq);
        ctx->dol_seq = NULL;
    }
    ctx->i = end;
    return (ctx->i);
}


int	handle_variable_type(t_ctx *ctx)
{
	int	result;
	int	tmp_i;

	if (ctx->inp[ctx->i] == '?')
	{
		tmp_i = ctx->i + 1;
		while (ctx->inp[tmp_i] && !isspace(ctx->inp[tmp_i]) && !is_operator(ctx->inp[tmp_i]))
		{
			if (ctx->inp[tmp_i] == '?')
			{
				result = handle_multiple_values(ctx);
				return (result);
			}
			tmp_i++;
		}
		result = gere_spcial_var(ctx);
		return (result);
	}
	if (ctx->inp[ctx->i] && ft_isdigit(ctx->inp[ctx->i]))
	{
		tmp_i = ctx->i;
		while (ctx->inp[tmp_i] && ft_isdigit(ctx->inp[tmp_i]))
			tmp_i++;
		if (ctx->inp[tmp_i] == '$' || ctx->inp[tmp_i] == '?')
		{
			result = handle_multiple_values(ctx);
			return (result);
		}
		result = gere_num_var(ctx);
		return (result);
	}
	if (ctx->inp[ctx->i] && (ft_isalnum(ctx->inp[ctx->i]) || ctx->inp[ctx->i] == '_'))
	{
		tmp_i = ctx->i;
		while (ctx->inp[tmp_i] && (ft_isalnum(ctx->inp[tmp_i]) || ctx->inp[tmp_i] == '_'))
			tmp_i++;
		if (ctx->inp[tmp_i] == '$' || ctx->inp[tmp_i] == '?')
		{
			result = handle_multiple_values(ctx);
			return (result);
		}
		result = gere_valid_var(ctx);
		return (result);
	}

	return (-1);
}



char	*initialize_exit_status(t_ctx *ctx)
{
	char	*exit_status;
	char	*token_value;

	exit_status = ft_itoa(ctx->e_l->exit_status);
	if (!exit_status)
		return (NULL);
	token_value = ft_strdup(exit_status);
	free(exit_status);
	return (token_value);
}

char	*accumulate_variable_value(t_ctx *ctx, char *token_value)
{
	char	temp[2];
	char	*new_token;

	while (ctx->inp[ctx->i + 1]
		&& !isspace(ctx->inp[ctx->i + 1])
		&& !is_operator(ctx->inp[ctx->i + 1]))
	{
		temp[0] = ctx->inp[++(ctx->i)];
		temp[1] = '\0';
		new_token = ft_strjoin(token_value, temp);
		free(token_value);
		token_value = new_token;
		if (!token_value)
			return (NULL);
	}
	return (token_value);
}

int	gere_spcial_var(t_ctx *ctx)
{
	char	*token_value;

	token_value = initialize_exit_status(ctx);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	token_value = accumulate_variable_value(ctx, token_value);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	add_token(ctx->tok, create_token(token_value, TYPE_WORD, 0));
	free(token_value);
	free(ctx->dol_seq);
	return (ctx->i + 1);
}
