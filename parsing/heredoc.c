#include "parsing.h"

// Crée un fichier temporaire pour stocker le contenu du heredoc
char *create_temp_file(char *content)
{
    char *filename = strdup("/tmp/heredocXXXXXX");
    int fd = mkstemp(filename);

    if (fd == -1)
    {
        perror("Erreur de création du fichier temporaire");
        free(filename);
        return (NULL);
    }

    write(fd, content, strlen(content));
    close(fd);
    return (filename);
}

// Concatène deux chaînes avec un saut de ligne entre elles
char *strjoin_with_newline(char *s1, char *s2)
{
    char *result;
    size_t len1 = s1 ? strlen(s1) : 0;
    size_t len2 = s2 ? strlen(s2) : 0;
    size_t total_len = len1 + len2 + 2; // +2 pour le saut de ligne et '\0'

    result = malloc(total_len);
    if (!result)
        return (NULL);

    if (s1)
        strcpy(result, s1);

    if (s2)
    {
        strcpy(result + len1, s2);
        result[len1 + len2] = '\n';
    }

    result[total_len - 1] = '\0';

    free(s1);
    return (result);
}

// Récupère le contenu d'un heredoc en lisant jusqu'au délimiteur
char *get_heredoc(char *delimiter)
{
    char *content = NULL;
    char *line = NULL;

    printf("> ");
    while ((line = readline(NULL)))
    {
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        content = strjoin_with_newline(content, line);
        free(line);
        printf("> ");
    }

    return (content);
}

// Traite un token heredoc et met à jour la structure `t_cmd_list`
int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd)
{
    // Vérifier qu'il y a un délimiteur après le token heredoc
    if (!(*tokens)->next || (*tokens)->next->type != TYPE_WORD)
    {
        fprintf(stderr, "Syntax error: missing heredoc delimiter\n");
        return (-1);
    }

    // Stocker le délimiteur dans heredoc_delimiter
    curr_cmd->heredoc_delimiter = ft_strdup((*tokens)->next->value);
    if (!curr_cmd->heredoc_delimiter)
    {
        fprintf(stderr, "Error: failed to allocate heredoc delimiter\n");
        return (-1);
    }

    // Récupérer le contenu du heredoc en demandant à l'utilisateur
    curr_cmd->heredoc_content = get_heredoc(curr_cmd->heredoc_delimiter);
    if (!curr_cmd->heredoc_content)
    {
        fprintf(stderr, "Error: failed to read heredoc content\n");
        free(curr_cmd->heredoc_delimiter);
        return (-1);
    }

    // Optionnel : Créer un fichier temporaire pour le heredoc et l'ajouter aux redirections
    char *temp_filename = create_temp_file(curr_cmd->heredoc_content);
    if (!temp_filename)
    {
        free(curr_cmd->heredoc_content);
        free(curr_cmd->heredoc_delimiter);
        return (-1);
    }

    // Ajouter le fichier temporaire aux fichiers de redirection
    curr_cmd->files_list = realloc(curr_cmd->files_list, sizeof(char *) * (curr_cmd->files_count + 1));
    curr_cmd->files_type = realloc(curr_cmd->files_type, sizeof(int) * (curr_cmd->files_count + 1));

    if (!curr_cmd->files_list || !curr_cmd->files_type)
    {
        free(temp_filename);
        free(curr_cmd->heredoc_content);
        free(curr_cmd->heredoc_delimiter);
        return (-1);
    }

    curr_cmd->files_list[curr_cmd->files_count] = temp_filename;
    curr_cmd->files_type[curr_cmd->files_count] = TYPE_HEREDOC;
    curr_cmd->files_count++;

    // Avancer au prochain token après le heredoc
    *tokens = (*tokens)->next->next;

    return (0);
}

