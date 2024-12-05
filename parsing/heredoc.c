#include "parsing.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>



char *create_temp_file(char *content)
{
    char *base_filename = "/tmp/heredoc";
    char *filename = malloc(strlen(base_filename) + 10);
    int fd, suffix = 0;

    if (!filename)
    {
        perror("Allocation error for temp file name");
        return NULL;
    }

    while (1)
    {
        sprintf(filename, "%s%d", base_filename, suffix);
        fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (fd != -1)
            break;

        if (errno != EEXIST)
        {
            perror("Error creating temp file");
            free(filename);
            return NULL;
        }
        suffix++;
    }

    if (write(fd, content, strlen(content)) == -1)
    {
        perror("Error writing to temp file");
        close(fd);
        unlink(filename);
        free(filename);
        return NULL;
    }

    close(fd);
    return filename;
}

char *strjoin_with_newline(char *s1, char *s2)
{
    size_t len1 = s1 ? strlen(s1) : 0;
    size_t len2 = s2 ? strlen(s2) : 0;
    size_t total_len = len1 + len2 + 2;

    char *result = malloc(total_len);
    if (!result)
        return NULL;

    if (s1)
        strcpy(result, s1);
    else
        result[0] = '\0';

    if (s2)
    {
        strcpy(result + len1, s2);
        result[len1 + len2] = '\n';
        result[len1 + len2 + 1] = '\0';
    }

    free(s1);
    return result;
}

void heredoc_child_sigint_handler(int sig)
{
    (void)sig;

    if (sig_status)
    {
        t_signal_context *ctx = sig_status;
        write(STDOUT_FILENO, "\n", 1); // Affiche une nouvelle ligne

        // Libération explicite des ressources
        if (ctx->tokens)
            free_tokens(*(ctx->tokens));
        if (ctx->delimiter)
            free(ctx->delimiter);
        if (ctx->data && ctx->env_list && ctx->curr_cmd)
            cleanup_resources(ctx->data, ctx->env_list, ctx->curr_cmd);

        if (ctx->pipefd_write >= 0)
            close(ctx->pipefd_write);
        exit(130); // Quitter proprement
    }
    else
    {
        write(STDOUT_FILENO, "\n", 1);
        exit(130); // Quitter proprement sans libération
    }
}



void setup_child_signal_handler(t_signal_context *ctx)
{
    struct sigaction sa_child;
    sa_child.sa_handler = heredoc_child_sigint_handler;
    sa_child.sa_flags = 0; // Pas besoin de SA_SIGINFO
    sig_status = ctx;
    sigemptyset(&sa_child.sa_mask);

    if (sigaction(SIGINT, &sa_child, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}




char *parent_process_heredoc(int pipefd[2], pid_t pid)
{
    int status;
    char buffer[1024];
    int bytes_read;
    char *heredoc_content = NULL;

    // Configurer le gestionnaire de SIGINT
    signal(SIGINT, sigint_handler);
    close(pipefd[1]); // Fermer l'écriture dans le parent

    heredoc_content = ft_strdup("");
    if (!heredoc_content)
    {
        perror("ft_strdup");
        close(pipefd[0]);
        return NULL;
    }
    // printf("DEBUG: Attente de la fin du processus enfant\n");

    waitpid(pid, &status, 0);
    // printf("DEBUG: Processus enfant terminé avec status: %d\n", status);
    // printf("DEBUG: Lecture depuis le pipe\n");
    while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        // printf("DEBUG: Contenu lu: %s\n", buffer);
        heredoc_content = strjoin_with_newline(heredoc_content, buffer);
    }

    close(pipefd[0]);

    // Restaurer le gestionnaire par défaut pour SIGINT après le heredoc
    signal(SIGINT, SIG_DFL);

    return heredoc_content;
}



void child_cleanup(t_signal_context *ctx, int pipefd_write)
{
    close(pipefd_write);
    free_tokens(*(ctx->tokens));
    free(ctx->delimiter);
    cleanup_resources(ctx->data, ctx->env_list, ctx->curr_cmd);
    // printf("DEBUG: Processus enfant terminé\n");
    exit(0);
}

void child_process_heredoc(t_signal_context *ctx, int pipefd[2])
{
    // printf("DEBUG: Processus enfant démarré\n");
    // Configurer le gestionnaire de signal avec le contexte
    setup_child_signal_handler(ctx);

    // Fermer la lecture dans l'enfant
    close(pipefd[0]);

    while (1)
    {
        char *line = readline("> ");
        if (!line)
        {
            // printf("DEBUG: EOF (Ctrl+D) détecté\n");
            child_cleanup(ctx, pipefd[1]);
        }
        else if (strcmp(line, ctx->delimiter) == 0)
        {
            // printf("DEBUG: Délimiteur trouvé, sortie de la boucle\n");
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    child_cleanup(ctx, pipefd[1]);
}







char *get_heredoc(char *delimiter, t_token **tokens, t_cmd_list *curr_cmd, t_data *data, t_env **env_list)
{
    int pipefd[2];
    pid_t pid;
    char *heredoc_content = NULL;
    t_signal_context ctx;

    // printf("DEBUG: Début de get_heredoc avec delimiter: %s\n", delimiter);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return NULL;
    }
    // printf("DEBUG: Pipe créé avec pipefd[0]: %d, pipefd[1]: %d\n", pipefd[0], pipefd[1]);

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }

    if (pid == 0)
    {
        ctx.tokens = tokens;
        ctx.delimiter = delimiter;
        ctx.data = data;
        ctx.env_list = env_list;
        ctx.curr_cmd = curr_cmd;
        ctx.pipefd_write = pipefd[1];

        // Exécution du processus enfant
        child_process_heredoc(&ctx, pipefd);
    }
    else
    {
        // Exécution du processus parent
        heredoc_content = parent_process_heredoc(pipefd, pid);
    }

    // printf("DEBUG: Retour du contenu heredoc\n");
    return heredoc_content;
}









int process_heredoc(t_token **tokens, t_cmd_list *curr_cmd, t_data *data, t_env **env_list)
{
    t_token *current = *tokens;

    while (current && current->type == TYPE_HEREDOC)
    {
        // Vérification de la syntaxe
        if (!current->next || current->next->type != TYPE_WORD)
        {
            fprintf(stderr, "syntax error near unexpected token 'newline'\n");
            return -1;
        }

        // Allocation pour le délimiteur du heredoc
        char *heredoc_delimiter = strdup(current->next->value);
        if (!heredoc_delimiter)
        {
            fprintf(stderr, "Error: failed to allocate heredoc delimiter\n");
            return -1;
        }

        // Lecture du contenu du heredoc
        char *heredoc_content = get_heredoc(heredoc_delimiter, tokens, curr_cmd, data, env_list);
        free(heredoc_delimiter);

        // Vérification des erreurs de lecture
        if (!heredoc_content)
        {
            fprintf(stderr, "Error: failed to read heredoc content\n");
            exit(1);
        }

        // Création d'un fichier temporaire
        char *temp_filename = create_temp_file(heredoc_content);
        free(heredoc_content); // Libérer heredoc_content après création du fichier

        if (!temp_filename)
        {
            fprintf(stderr, "Error: failed to create heredoc temp file\n");
            return -1;
        }

        // Extension des listes de fichiers et types
        char **new_files_list = realloc(curr_cmd->files_list, sizeof(char *) * (curr_cmd->files_count + 2));
        int *new_files_type = realloc(curr_cmd->files_type, sizeof(int) * (curr_cmd->files_count + 1));

        if (!new_files_list || !new_files_type)
        {
            free(temp_filename);
            fprintf(stderr, "Error: failed to allocate memory for files list\n");
            free_cmd_list(curr_cmd);
            free_tokens(*tokens);
            return -1;
        }

        // Mise à jour des listes
        curr_cmd->files_list = new_files_list;
        curr_cmd->files_type = new_files_type;
        curr_cmd->files_list[curr_cmd->files_count] = temp_filename;
        curr_cmd->files_type[curr_cmd->files_count] = TYPE_HEREDOC;
        curr_cmd->files_list[curr_cmd->files_count + 1] = NULL;

        curr_cmd->files_count++;

        // Passage au token suivant
        current = current->next->next;
    }

    // Mise à jour du pointeur des tokens
    *tokens = current;
    return 0;
}
