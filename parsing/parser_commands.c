#include "parsing.h"

// Fonction pour créer un nouveau nœud de commande
t_cmd_list *create_cmd_node(void)
{
    t_cmd_list *new_node = malloc(sizeof(t_cmd_list));
    if (!new_node)
        return (NULL);
    new_node->cmd_args = NULL;
    new_node->files_list = NULL;
    new_node->files_type = NULL;
    new_node->last_in = -1;
    new_node->last_out = -1;
    new_node->next = NULL;
    return new_node;
}

char **append_file(char **files_list, char *file)
{
    int len = 0;
    while (files_list && files_list[len])
        len++;

    char **new_list = realloc(files_list, sizeof(char *) * (len + 2)); // +2 pour le nouvel élément et NULL
    if (!new_list)
        return NULL; // Gérer l'erreur d'allocation

    new_list[len] = ft_strdup(file);
    new_list[len + 1] = NULL;

    return new_list;
}

int *append_file_type(int *files_type, int type)
{
    int len = 0;
    while (files_type && files_type[len])
        len++;

    int *new_types = realloc(files_type, sizeof(int) * (len + 1));
    if (!new_types)
        return NULL; // Gérer l'erreur d'allocation

    new_types[len] = type;

    return new_types;
}

int get_last_index(char **list)
{
    int len = 0;
    while (list && list[len])
        len++;

    return len - 1; // Retourner l'index du dernier élément
}



int count_tokens(t_token *tokens)
{
    int count = 0;
    
    while (tokens && tokens->type != TYPE_PIPE) // On arrête au pipe
    {
        if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED) // Vous pouvez ajuster selon vos types
            count++;
        tokens = tokens->next;
    }
    return count;
}

t_cmd_list *parse_commands(t_token *tokens)
{
    t_cmd_list *cmd_list = NULL;
    t_cmd_list *current_cmd = NULL;

    while (tokens)
    {
        // Création d'un nouveau nœud de commande
        if (!cmd_list)
        {
            cmd_list = create_cmd_node();
            current_cmd = cmd_list;
        }
        else
        {
            current_cmd->next = create_cmd_node();
            current_cmd = current_cmd->next;
        }

        // Initialiser last_in et last_out à -1 (pas de redirection par défaut)
        current_cmd->last_in = -1;
        current_cmd->last_out = -1;

        // Initialisation des listes de fichiers et types
        current_cmd->files_list = NULL;
        current_cmd->files_type = NULL;

        // Variables pour construire cmd_args
        t_token *start = tokens;
        int arg_count = 0;

        // Compter le nombre d'arguments pour cmd_args (hors redirections)
        t_token *tmp = tokens;
        while (tmp && tmp->type != TYPE_PIPE)
        {
            if (tmp->type == TYPE_WORD || tmp->type == TYPE_QUOTED)
                arg_count++;
            else if (tmp->type == TYPE_REDIRECTION_INPUT || tmp->type == TYPE_REDIRECTION_OUTPUT || tmp->type == TYPE_REDIRECTION_APPEND || tmp->type == TYPE_HEREDOC)
            {
                // Sauter le token de redirection et le fichier suivant
                tmp = tmp->next;
                if (tmp)
                    tmp = tmp->next;
                continue;
            }
            tmp = tmp->next;
        }

        // Allocation de cmd_args
        current_cmd->cmd_args = malloc(sizeof(char *) * (arg_count + 1));
        if (!current_cmd->cmd_args)
            return NULL; // Gérer l'erreur d'allocation

        // Remplir cmd_args et gérer les redirections
        int arg_index = 0;
        tokens = start;
        while (tokens && tokens->type != TYPE_PIPE)
        {
            if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED)
            {
                current_cmd->cmd_args[arg_index++] = ft_strdup(tokens->value);
                tokens = tokens->next;
            }
            else if (tokens->type == TYPE_REDIRECTION_INPUT)
            {
                tokens = tokens->next;
                if (tokens && (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED))
                {
                    // Ajouter le fichier infile
                    current_cmd->files_list = append_file(current_cmd->files_list, tokens->value);
                    current_cmd->files_type = append_file_type(current_cmd->files_type, 0); // 0 pour infile
                    current_cmd->last_in = get_last_index(current_cmd->files_list); // Mettre à jour last_in
                    tokens = tokens->next;
                }
                else
                {
                    // Gérer l'erreur de syntaxe : fichier manquant après '<'
                    printf("Erreur : fichier manquant après '<'\n");
                    return NULL;
                }
            }
            else if (tokens->type == TYPE_REDIRECTION_OUTPUT || tokens->type == TYPE_REDIRECTION_APPEND)
            {
                tokens = tokens->next;
                if (tokens && (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED))
                {
                    // Ajouter le fichier outfile
                    current_cmd->files_list = append_file(current_cmd->files_list, tokens->value);
                    current_cmd->files_type = append_file_type(current_cmd->files_type, 1); // 1 pour outfile
                    current_cmd->last_out = get_last_index(current_cmd->files_list); // Mettre à jour last_out
                    tokens = tokens->next;
                }
                else
                {
                    // Gérer l'erreur de syntaxe : fichier manquant après '>' ou '>>'
                    printf("Erreur : fichier manquant après '>' ou '>>'\n");
                    return NULL;
                }
            }
            else
            {
                // Autres types de tokens (à gérer selon vos besoins)
                tokens = tokens->next;
            }
        }

        // Terminer cmd_args par NULL
        current_cmd->cmd_args[arg_index] = NULL;

        // Initialiser le champ 'cmd' avec le premier argument
        if (current_cmd->cmd_args && current_cmd->cmd_args[0])
        {
            current_cmd->cmd = ft_strdup(current_cmd->cmd_args[0]);
        }

        // Passer au token suivant si un pipe est rencontré
        if (tokens && tokens->type == TYPE_PIPE)
        {
            printf("Pipe détecté, prochaine commande\n");
            tokens = tokens->next;
        }
    }
	//print_cmd_list(cmd_list);
    return cmd_list;
}




void print_cmd_list(t_cmd_list *cmd_list)
{
    while (cmd_list)
    {
        // Afficher la commande
        printf("Commande : %s\n", cmd_list->cmd);

        // Afficher les arguments de la commande
        printf("Arguments de la commande : \n");
        for (int i = 0; cmd_list->cmd_args && cmd_list->cmd_args[i]; i++)
        {
            printf("cmd_args[%d] = %s\n", i, cmd_list->cmd_args[i]);
        }

        // Afficher les fichiers de redirection
        printf("Liste des fichiers (infile et outfile) : \n");
        for (int i = 0; cmd_list->files_list && cmd_list->files_list[i]; i++)
        {
            const char *type_str = (cmd_list->files_type[i] == 0) ? "infile" : "outfile";
            printf("files_list[%d] = %s (type = %d : %s)\n", i, cmd_list->files_list[i], cmd_list->files_type[i], type_str);
        }

        // Afficher last_in et last_out
        if (cmd_list->last_in != -1)
            printf("last_in = files_list[%d]\n", cmd_list->last_in);
        else
            printf("last_in = -1\n");

        if (cmd_list->last_out != -1)
            printf("last_out = files_list[%d]\n", cmd_list->last_out);
        else
            printf("last_out = -1\n");

        // Passer à la commande suivante
        cmd_list = cmd_list->next;

        printf("\n"); // Séparation entre les commandes
    }
}




// Fonction pour libérer la mémoire de la liste chaînée de commandes
//POUR L'INSTANT JE NE M'EN SERS PAS. 
void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_list *tmp;

    while (cmd_list)
    {
        tmp = cmd_list;
        cmd_list = cmd_list->next;

        // Libérer les arguments de la commande
        if (tmp->cmd_args)
        {
            int i = 0;
            while (tmp->cmd_args[i])
            {
                free(tmp->cmd_args[i]);
                i++;
            }
            free(tmp->cmd_args);
        }

        // Libérer la liste des fichiers (infiles/outfiles)
        if (tmp->files_list)
        {
            int i = 0;
            while (tmp->files_list[i])
            {
                free(tmp->files_list[i]);
                i++;
            }
            free(tmp->files_list);
        }

        // Libérer les types de fichiers (infile/outfile)
        if (tmp->files_type)
            free(tmp->files_type);

        // Libérer le nœud lui-même
        free(tmp);
    }
}