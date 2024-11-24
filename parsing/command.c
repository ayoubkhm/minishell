#include "parsing.h"

t_cmd_list	*create_cmd_node(void)
{
	t_cmd_list	*new_node;

	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
	new_node->cmd_args = NULL;
	new_node->cmd = NULL;
	new_node->files_list = NULL;
	new_node->files_type = NULL;
	new_node->files_count = 0;
	new_node->last_in = -1;
	new_node->last_out = -1;
	new_node->next = NULL;
	new_node->prev = NULL; 
	return (new_node);
}

t_cmd_list *init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd)
{
    if (!(*cmd_list))
    {
        *cmd_list = create_cmd_node();
        *current_cmd = *cmd_list;
    }
    else
    {
        (*current_cmd)->next = create_cmd_node();
        (*current_cmd)->next->prev = *current_cmd;
        *current_cmd = (*current_cmd)->next;
    }
    (*current_cmd)->last_in = -1;
    (*current_cmd)->last_out = -1;
    return (*current_cmd);
}


int	allocate_command_args(t_cmd_list *current_cmd, int arg_count)
{
	current_cmd->cmd_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!current_cmd->cmd_args)
		return (-1);
	return (0);
}

void post_process_command(t_cmd_list *current_cmd, t_env **env_list)
{
    if (current_cmd->cmd_args && current_cmd->cmd_args[0])
    {
        if (current_cmd->cmd) // Libère l'ancienne commande si elle existe
        {
            free(current_cmd->cmd);
            current_cmd->cmd = NULL;
        }
        current_cmd->cmd = strdup(current_cmd->cmd_args[0]);
        if (strcmp(current_cmd->cmd, "export") == 0)
        {
            handle_export(current_cmd, env_list);
        }
    }
}


void print_commands(t_cmd_list *cmd_list)
{
    t_cmd_list *current_cmd = cmd_list;
    int i;

    while (current_cmd)
    {
        printf("Commande : %s\n", current_cmd->cmd ? current_cmd->cmd : "NULL");

        printf("Arguments :\n");
        if (current_cmd->cmd_args)
        {
            for (i = 0; current_cmd->cmd_args[i]; i++)
            {
                printf("  arg[%d]: %s\n", i, current_cmd->cmd_args[i]);
            }
        }
        else
        {
            printf("  Aucun argument\n");
        }

        printf("Fichiers de redirection :\n");
        if (current_cmd->files_list && current_cmd->files_count > 0)
        {
            for (i = 0; i < current_cmd->files_count; i++)
            {
                char *type_str;
                if (current_cmd->files_type[i] == TYPE_REDIR_IN)
                    type_str = "Entrée";
                else if (current_cmd->files_type[i] == TYPE_REDIR_OUT)
                    type_str = "Sortie";
                else if (current_cmd->files_type[i] == TYPE_REDIR_APPEND)
                    type_str = "Append";
                else if (current_cmd->files_type[i] == TYPE_HEREDOC)
                    type_str = "Heredoc";
                else
                    type_str = "Inconnu";

                printf("  file[%d]: %s (type: %s)\n", i, current_cmd->files_list[i], type_str);
            }
        }
        else
        {
            printf("  Aucun fichier de redirection\n");
        }

        if (current_cmd->heredoc_delimiter)
            printf("Heredoc delimiter: %s\n", current_cmd->heredoc_delimiter);

        if (current_cmd->heredoc_content)
            printf("Heredoc content:\n%s\n", current_cmd->heredoc_content);

        printf("last_in: %d, last_out: %d\n", current_cmd->last_in, current_cmd->last_out);

        printf("prev: %p, current: %p, next: %p\n",
               (void *)current_cmd->prev, (void *)current_cmd, (void *)current_cmd->next);

        printf("---------------------\n");

        current_cmd = current_cmd->next;
    }
}



t_cmd_list *parse_commands(t_token *tokens, t_env **env_list)
{
    t_cmd_list *cmd_list;
    t_cmd_list *current_cmd;
    int arg_count;
    int ret; // Variable pour stocker le retour de process_token_cmd

    cmd_list = NULL;
    current_cmd = NULL;
    while (tokens)
    {
        current_cmd = init_command_node(&cmd_list, &current_cmd);
        arg_count = count_arguments(tokens);
        if (allocate_command_args(current_cmd, arg_count) == -1)
            return (NULL);
        
        ret = process_token_cmd(&tokens, current_cmd, *env_list);        
        if (ret == -1)
            return (NULL);

        post_process_command(current_cmd, env_list);

        if (tokens && tokens->type == TYPE_PIPE)
        {
            tokens = tokens->next;
        }
    }
    if (current_cmd && current_cmd->cmd)
    {
        free(current_cmd->cmd);
        current_cmd->cmd = NULL;
    }
    return cmd_list;
}
