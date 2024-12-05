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


int process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd)
{
    int arg_index = 0;

    while (*tokens && (*tokens)->type != TYPE_PIPE)
    {
        // Si le token est un argument à ajouter à `cmd_args`
        if ((*tokens)->type == TYPE_WORD || (*tokens)->type == TYPE_QUOTED || (*tokens)->type == TYPE_ENV_VAR)
        {
            char *arg_value = ft_strdup((*tokens)->value); // Directement utiliser la valeur du token

            // Vérifier l'allocation
            if (!arg_value)
            {
                fprintf(stderr, "minishell: memory allocation failed for argument\n");
                return (-1);
            }

            // printf("DEBUG: Ajout de l'argument '%s' à cmd_args[%d]\n", arg_value, arg_index);

            // Ajouter l'argument à `cmd_args` et gérer l'indexation
            curr_cmd->cmd_args[arg_index++] = arg_value;

            // Avancer le pointeur tokens
            *tokens = (*tokens)->next;
        }
        else if ((*tokens)->type == TYPE_REDIR_IN || (*tokens)->type == TYPE_REDIR_OUT || 
                 (*tokens)->type == TYPE_REDIR_APPEND || (*tokens)->type == TYPE_HEREDOC)
        {
            // Gestion spécifique pour heredocs
            if ((*tokens)->type == TYPE_HEREDOC)
            {
                // printf("DEBUG: Traitement d'un heredoc\n");
                if (process_heredoc(tokens, curr_cmd) == -1)
                {
                    return (-1);
                }
            }
            else
            {
                // Si c'est une redirection classique, appeler `process_redirections`
                // printf("DEBUG: Traitement d'une redirection classique\n");
                if (process_redirections(tokens, curr_cmd) == -1)
                    return (-1);
            }
        }
        else
        {
            // Si le token n'est ni un argument ni une redirection, on avance simplement
            // printf("DEBUG: Ignorer le token de type %d\n", (*tokens)->type);
            *tokens = (*tokens)->next;
        }
    }

    // Terminer la liste des arguments avec NULL
    curr_cmd->cmd_args[arg_index] = NULL;
    // printf("DEBUG: Fin de process_token_cmd, cmd_args terminé avec %d arguments\n", arg_index);
    return (0);
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
