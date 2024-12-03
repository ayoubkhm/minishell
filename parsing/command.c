#include "parsing.h"

t_cmd_list	*create_cmd_node(void)
{
	t_cmd_list	*new_node;

	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
    new_node->save_std[0] = -1;
    new_node->save_std[1] = -1;
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

void remove_empty_arguments(t_cmd_list *current_cmd)
{
    if (!current_cmd || !current_cmd->cmd_args)
        return;

    int arg_count = 0;
    int new_arg_count = 0;

    // Comptez les arguments non vides
    while (current_cmd->cmd_args[arg_count])
    {
        if (current_cmd->cmd_args[arg_count][0] != '\0')
            new_arg_count++;
        arg_count++;
    }

    // Si tous les arguments sont valides, rien à faire
    if (new_arg_count == arg_count)
        return;

    // Créez une nouvelle liste pour les arguments valides
    char **new_cmd_args = malloc(sizeof(char *) * (new_arg_count + 1));
    if (!new_cmd_args)
        return;

    int j = 0;
    for (int i = 0; i < arg_count; i++)
    {
        if (current_cmd->cmd_args[i][0] != '\0')
        {
            new_cmd_args[j] = current_cmd->cmd_args[i];
            j++;
        }
        else
        {
            free(current_cmd->cmd_args[i]); // Libérez les arguments vides
        }
    }
    new_cmd_args[j] = NULL; // Terminez avec NULL

    // Remplacez l'ancienne liste par la nouvelle
    free(current_cmd->cmd_args);
    current_cmd->cmd_args = new_cmd_args;
}


void remove_env_variable(char *var_name, t_env **env_list)
{
    t_env *current = *env_list;
    t_env *prev = NULL;

    while (current)
    {
        if (strcmp(current->name, var_name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;

            free(current->name);
            free(current->value);
            free(current);
            return; // Sort dès qu'on trouve la variable
        }
        prev = current;
        current = current->next;
    }
}


void handle_unset(t_cmd_list *cmd, t_env **env_list)
{
    int i = 1;
    while (cmd->cmd_args[i])
    {
        remove_env_variable(cmd->cmd_args[i], env_list);
        i++;
    }
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
        else if (strcmp(current_cmd->cmd, "unset") == 0)
        {
            handle_unset(current_cmd, env_list);
        }
    }

    // Supprime les arguments vides
    remove_empty_arguments(current_cmd);
}



void print_commands(t_cmd_list *cmd_list)
{
    t_cmd_list *current_cmd = cmd_list;

    while (current_cmd)
    {
        // Affichage de la commande
        printf("Commande : %s\n", current_cmd->cmd ? current_cmd->cmd : "NULL");

        // Affichage des arguments
        printf("Arguments :\n");
        if (current_cmd->cmd_args)
        {
            int i = 0;
            while (current_cmd->cmd_args[i])
            {
                printf("  arg[%d]: %s\n", i, current_cmd->cmd_args[i]);
                i++;
            }
        }
        else
        {
            printf("  Aucun argument\n");
        }

        // Affichage des fichiers de redirection simplifié
        printf("Fichiers de redirection :\n");
        if (current_cmd->files_list && current_cmd->files_count > 0)
        {
            for (int i = 0; i < current_cmd->files_count; i++)
            {
                printf("  file[%d]: %s\n",
                       i,
                       current_cmd->files_list[i] ? current_cmd->files_list[i] : "NULL");
            }
        }
        else
        {
            printf("  Aucun fichier de redirection\n");
        }

        // Affichage des descripteurs last_in et last_out
        printf("last_in: %d, last_out: %d\n",
               current_cmd->last_in,
               current_cmd->last_out);

        // Affichage des pointeurs
        printf("prev: %p, current: %p, next: %p\n",
               (void *)current_cmd->prev,
               (void *)current_cmd,
               (void *)current_cmd->next);

        printf("---------------------\n");

        // Passage au prochain noeud
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
