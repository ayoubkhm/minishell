#include "parsing.h"

char *create_temp_file(char *content)
{
    char *base_filename = "/tmp/heredoc";
    char *filename;
    int fd;
    int suffix = 0;

    filename = malloc(strlen(base_filename) + 10);
    if (!filename)
    {
        perror("Erreur d'allocation de mémoire pour le nom du fichier");
        return (NULL);
    }

    while (1)
    {
        sprintf(filename, "%s%d", base_filename, suffix);
        fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (fd != -1)
            break;

        if (errno != EEXIST)
        {
            perror("Erreur de création du fichier temporaire");
            free(filename);
            return (NULL);
        }
        suffix++;
    }

    if (write(fd, content, strlen(content)) == -1)
    {
        perror("Erreur d'écriture dans le fichier temporaire");
        close(fd);
        unlink(filename);
        free(filename);
        return (NULL);
    }

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

int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd)
{
    t_token *current = *tokens;

    while (current && current->type == TYPE_HEREDOC)
    {
        // Vérifier qu'il y a un délimiteur après le token heredoc
        if (!current->next || current->next->type != TYPE_WORD)
        {
            fprintf(stderr, "syntax error near unexpected token 'newline'\n");
            return (-1);
        }

        // Stocker le délimiteur
        char *heredoc_delimiter = ft_strdup(current->next->value);
        if (!heredoc_delimiter)
        {
            fprintf(stderr, "Error: failed to allocate heredoc delimiter\n");
            return (-1);
        }

        // Récupérer le contenu du heredoc
        char *heredoc_content = get_heredoc(heredoc_delimiter);
        free(heredoc_delimiter);
        if (!heredoc_content)
        {
            fprintf(stderr, "Error: failed to read heredoc content\n");
            return (-1);
        }

        // Créer un fichier temporaire pour le heredoc
        char *temp_filename = create_temp_file(heredoc_content);
        free(heredoc_content);
        if (!temp_filename)
        {
            fprintf(stderr, "Error: failed to create heredoc temp file\n");
            return (-1);
        }

        // Ajouter le fichier temporaire aux fichiers de redirection
        // +1 pour le nouveau fichier, +1 pour le pointeur NULL à la fin
        char **new_files_list = realloc(curr_cmd->files_list, sizeof(char *) * (curr_cmd->files_count + 2));
        int *new_files_type = realloc(curr_cmd->files_type, sizeof(int) * (curr_cmd->files_count + 1));

        if (!new_files_list || !new_files_type)
        {
            free(temp_filename);
            fprintf(stderr, "Error: failed to allocate memory for files list\n");
            return (-1);
        }

        curr_cmd->files_list = new_files_list;
        curr_cmd->files_type = new_files_type;

        // Ajouter le fichier temporaire à la liste
        curr_cmd->files_list[curr_cmd->files_count] = temp_filename;
        curr_cmd->files_type[curr_cmd->files_count] = TYPE_HEREDOC;

        // Terminer la liste par NULL
        curr_cmd->files_list[curr_cmd->files_count + 1] = NULL;

        // Incrémenter le compteur
        curr_cmd->files_count++;

        // Avancer au prochain token
        current = current->next->next;
    }
    // Mettre à jour le pointeur des tokens
    *tokens = current;

    return (0);
}