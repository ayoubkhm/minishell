#include "parsing.h"

int	proc_com_args(t_token **tok, t_cmd_list *c_cmd, int *arg_i)
{
    // Pas d'expansion ici, on copie directement la valeur du token
    if ((*tok)->type == TYPE_WORD || (*tok)->type == TYPE_QUOTED || (*tok)->type == TYPE_ENV_VAR)
    {
        // Ajout de la valeur du token à cmd_args sans expansion
        c_cmd->cmd_args[(*arg_i)++] = ft_strdup((*tok)->value);
        *tok = (*tok)->next;
        return (1);
    }
    return (0);
}


int	process_redirections(t_token **tokens, t_cmd_list *current_cmd)
{
	if ((*tokens)->type == TYPE_REDIR_IN
		|| (*tokens)->type == TYPE_REDIR_OUT
		|| (*tokens)->type == TYPE_REDIR_APPEND)
	{
		if (handle_redirection_w_token(tokens, current_cmd) == -1)
			return (-1);
	}
	return (0);
}


int process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd, t_env *env_list)
{
    int arg_index = 0;

    // Vérifier que le token courant est un nom de commande valide
    if (*tokens && ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_QUOTED || (*tokens)->type == TYPE_ENV_VAR))
    {
        char *cmd_name;

        // Expansion si nécessaire
        if ((*tokens)->expand)
            cmd_name = expand_variables((*tokens)->value, env_list);
        else
            cmd_name = ft_strdup((*tokens)->value);

        if (!cmd_name)
            return -1;

        curr_cmd->cmd_args[arg_index++] = cmd_name; // Ajoute le nom de la commande à cmd_args[0]
        *tokens = (*tokens)->next;
    }
    else
    {
        // Pas de commande trouvée
        fprintf(stderr, "minishell: syntax error: no command found\n");
        return -1;
    }

    // Traiter les arguments restants
    while (*tokens && (*tokens)->type != TYPE_PIPE)
    {
        if ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_QUOTED || (*tokens)->type == TYPE_ENV_VAR)
        {
            char *arg_value = ft_strdup("");
            if (!arg_value)
                return -1;

            // Boucle pour concaténer les tokens adjacents de type WORD, QUOTED ou ENV_VAR
            while (*tokens && ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_QUOTED || (*tokens)->type == TYPE_ENV_VAR))
            {
                char *temp_value;

                // Expansion si nécessaire
                if ((*tokens)->expand)
                    temp_value = expand_variables((*tokens)->value, env_list);
                else
                    temp_value = ft_strdup((*tokens)->value);

                if (!temp_value)
                {
                    free(arg_value);
                    return -1;
                }

                // Concatène temp_value à arg_value
                char *temp = arg_value;
                arg_value = ft_strjoin(arg_value, temp_value);
                free(temp_value);
                free(temp);

                // Avancer le pointeur tokens
                *tokens = (*tokens)->next;
            }

            // Ajouter arg_value à cmd_args
            curr_cmd->cmd_args[arg_index++] = arg_value;
        }
        else if ((*tokens)->type == TYPE_REDIR_IN || (*tokens)->type == TYPE_REDIR_OUT ||
                 (*tokens)->type == TYPE_REDIR_APPEND || (*tokens)->type == TYPE_HEREDOC)
        {
            // Gestion des redirections
            if ((*tokens)->type == TYPE_HEREDOC)
            {
                if (process_heredoc(tokens, curr_cmd) == -1)
                    return -1;
            }
            else
            {
                if (process_redirections(tokens, curr_cmd) == -1)
                    return -1;
            }
            // Les fonctions de redirection doivent avancer le pointeur tokens
        }
        else
        {
            // Avancer le pointeur tokens pour éviter une boucle infinie
            *tokens = (*tokens)->next;
        }
    }

    // Terminer la liste des arguments avec NULL
    curr_cmd->cmd_args[arg_index] = NULL;
    return 0;
}




int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TYPE_PIPE)
	{
		if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	count_arguments(t_token *tokens)
{
	int		arg_count;
	t_token	*tmp;

	arg_count = 0;
	tmp = tokens;
	while (tmp && tmp->type != TYPE_PIPE)
	{
		if (tmp->type == TYPE_WORD
			|| tmp->type == TYPE_QUOTED
			|| tmp->type == TYPE_ENV_VAR)
			arg_count++;
		else if (tmp->type == TYPE_REDIR_IN || tmp->type == TYPE_REDIR_OUT
			|| tmp->type == TYPE_REDIR_APPEND
			|| tmp->type == TYPE_HEREDOC)
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		tmp = tmp->next;
	}
	return (arg_count);
}
