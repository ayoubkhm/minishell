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

char *strjoin_with_newline(char *s1, char *s2)
{
    char *result;
    size_t len1 = 0;
    size_t len2 = 0;
    size_t total_len;

    if (s1)
        len1 = strlen(s1);
    if (s2)
        len2 = strlen(s2);

    total_len = len1 + len2 + 2; // +2 pour le '\n' et le '\0'

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

char *get_heredoc_delimiter(t_token *current)
{
    if (!current->next || current->next->type != TYPE_WORD)
    {
        fprintf(stderr, "Syntax error: missing heredoc delimiter\n");
        return (NULL);
    }
    char *delimiter = ft_strdup(current->next->value);
    if (!delimiter)
    {
        fprintf(stderr, "Error: failed to allocate heredoc delimiter\n");
    }
    return (delimiter);
}

char *create_temp_file_with_content(char *content)
{
    if (!content)
    {
        content = strdup("");
        if (!content)
        {
            fprintf(stderr, "Error: failed to allocate memory for empty heredoc content\n");
            return (NULL);
        }
    }
    char *filename = create_temp_file(content);
    if (!filename)
    {
        fprintf(stderr, "Error: failed to create heredoc temp file\n");
    }
    return (filename);
}

int add_temp_file_to_cmd(t_cmd_list *curr_cmd, char *temp_filename)
{
    size_t old_files_size = sizeof(char *) * (curr_cmd->files_count + 1);
    size_t new_files_size = sizeof(char *) * (curr_cmd->files_count + 2);
    char **new_files_list = ft_realloc(curr_cmd->files_list, old_files_size, new_files_size);
    int *new_files_type = ft_realloc(curr_cmd->files_type, old_types_size, new_types_size);

    if (!new_files_list)
    {
        free(temp_filename);
        return (-1);
    }
    if (!new_files_type)
    {
        free(temp_filename);
        free(new_files_list);
        return (-1);
    }
    curr_cmd->files_list = new_files_list;
    curr_cmd->files_type = new_files_type;
    curr_cmd->files_list[curr_cmd->files_count] = temp_filename;
    curr_cmd->files_type[curr_cmd->files_count] = TYPE_HEREDOC;
    curr_cmd->files_list[curr_cmd->files_count + 1] = NULL;
    curr_cmd->files_count++;
    return (0);
}


int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd)
{
    t_token *current = *tokens;

    while (current && current->type == TYPE_HEREDOC)
    {
        // Étape 1 : Récupérer le délimiteur
        char *heredoc_delimiter = get_heredoc_delimiter(current);
        if (!heredoc_delimiter)
            return (-1);

        // Étape 2 : Récupérer le contenu du heredoc
        char *heredoc_content = get_heredoc(heredoc_delimiter);
        free(heredoc_delimiter);

        // Étape 3 : Créer un fichier temporaire
        char *temp_filename = create_temp_file_with_content(heredoc_content);
        free(heredoc_content);
        if (!temp_filename)
            return (-1);

        // Étape 4 : Ajouter le fichier temporaire aux redirections
        if (add_temp_file_to_cmd(curr_cmd, temp_filename) == -1)
            return (-1);

        // Avancer au prochain token
        current = current->next->next;
    }

    *tokens = current;
    return (0);
}
