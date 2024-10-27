#include "parsing.h"

t_cmd_list *create_cmd_node(void)
{
    t_cmd_list *new_node = malloc(sizeof(t_cmd_list));
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
    return new_node;
}


void set_env_variable(t_env_var **env_list, char *name, char *value)
{
    t_env_var *current = *env_list;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            free(current->value);
            current->value = strdup(value);
            return;
        }
        current = current->next;
    }
    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = *env_list;
    *env_list = new_var;
}

void handle_export(t_cmd_list *cmd, t_env_var **env_list)
{
    for (int i = 1; cmd->cmd_args[i]; i++)
    {
        char *arg = cmd->cmd_args[i];
        char *equal_sign = strchr(arg, '=');
        if (equal_sign) {
            size_t name_len = equal_sign - arg;
            char *name = strndup(arg, name_len);
            char *value = strdup(equal_sign + 1);
            set_env_variable(env_list, name, value);
            free(name);
            free(value);
        }
    }
}

char *get_env_variable(t_env_var *env_list, char *name)
{
    t_env_var *current = env_list;
    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

char *expand_variables(char *arg, t_env_var *env_list)
{
    char *result = strdup("");
    int i = 0;
    while (arg[i])
    {
        if (arg[i] == '$')
        {
            i++;
            int var_start = i;
            while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
                i++;
            char *var_name = ft_strndup(&arg[var_start], i - var_start);
            char *var_value = get_env_variable(env_list, var_name);
            if (var_value)
            {
                char *temp = result;
                result = ft_strjoin(result, var_value);
                free(temp);
            }
            free(var_name);
        }
        else
        {
            char temp_str[2] = {arg[i], '\0'};
            char *temp = result;
            result = ft_strjoin(result, temp_str);
            free(temp);
            i++;
        }
    }
    return result;
}

void add_redirection(t_cmd_list *cmd, char *filename, int type)
{
    int new_count = cmd->files_count + 1;
    char **new_files_list = realloc(cmd->files_list, sizeof(char *) * new_count);
    if (!new_files_list)
    {
        return;
    }
    cmd->files_list = new_files_list;
    cmd->files_list[cmd->files_count] = ft_strdup(filename);
    int *new_files_type = realloc(cmd->files_type, sizeof(int) * new_count);
    if (!new_files_type)
    {
        return;
    }
    cmd->files_type = new_files_type;
    cmd->files_type[cmd->files_count] = type;
    if (type == 0)
        cmd->last_in = cmd->files_count;
    else if (type == 1 || type == 2)
        cmd->last_out = cmd->files_count;
    cmd->files_count = new_count;
}

char **append_file(char **files_list, char *file)
{
    int len = 0;
    while (files_list && files_list[len])
        len++;
    char **new_list = realloc(files_list, sizeof(char *) * (len + 2));
    if (!new_list)
        return NULL;
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
        return NULL;
    new_types[len] = type;
    return new_types;
}

int get_last_index(char **list)
{
    int len = 0;
    while (list && list[len])
        len++;
    return len - 1;
}

int count_tokens(t_token *tokens)
{
    int count = 0;
    
    while (tokens && tokens->type != TYPE_PIPE)
    {
        if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED)
            count++;
        tokens = tokens->next;
    }
    return count;
}

t_cmd_list *parse_commands(t_token *tokens, t_env_var **env_list)
{
    t_cmd_list *cmd_list = NULL;
    t_cmd_list *current_cmd = NULL;

    while (tokens)
    {
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
        current_cmd->last_in = -1;
        current_cmd->last_out = -1;
        t_token *start = tokens;
        int arg_count = 0;
        t_token *tmp = tokens;
        while (tmp && tmp->type != TYPE_PIPE)
        {
            if (tmp->type == TYPE_WORD || tmp->type == TYPE_QUOTED || tmp->type == 5) 
                arg_count++;
            else if (tmp->type == TYPE_REDIRECTION_INPUT || tmp->type == TYPE_REDIRECTION_OUTPUT ||
                     tmp->type == TYPE_REDIRECTION_APPEND || tmp->type == TYPE_HEREDOC)
            {
                tmp = tmp->next;
                if (tmp)
                    tmp = tmp->next;
                continue;
            }
            tmp = tmp->next;
        }
        current_cmd->cmd_args = malloc(sizeof(char *) * (arg_count + 1));
        if (!current_cmd->cmd_args)
            return NULL;
        int arg_index = 0;
        tokens = start;
        while (tokens && tokens->type != TYPE_PIPE)
        {
            if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED || tokens->type == TYPE_ENV_VAR)
            {
                char *expanded_arg = expand_variables(tokens->value, *env_list);
                current_cmd->cmd_args[arg_index++] = expanded_arg;
                tokens = tokens->next;
            }
            else if (tokens->type == TYPE_REDIRECTION_INPUT)
            {
                tokens = tokens->next;
                if (tokens && (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED))
                {
                    add_redirection(current_cmd, tokens->value, 0);
                    tokens = tokens->next;
                }
                else
                    return NULL;
            }
            else if (tokens->type == TYPE_REDIRECTION_OUTPUT)
            {
                tokens = tokens->next;
                if (tokens && (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED))
                {
                    add_redirection(current_cmd, tokens->value, 1);
                    tokens = tokens->next;
                }
                else
                    return NULL;
            }
            else if (tokens->type == TYPE_REDIRECTION_APPEND)
            {
                tokens = tokens->next;
                if (tokens && (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED))
                {
                    add_redirection(current_cmd, tokens->value, 2);
                    tokens = tokens->next;
                }
                else
                    return NULL;
            }
            else
            {
                tokens = tokens->next;
            }
        }

        current_cmd->cmd_args[arg_index] = NULL;

        if (current_cmd->cmd_args && current_cmd->cmd_args[0])
        {
            current_cmd->cmd = strdup(current_cmd->cmd_args[0]);
            if (strcmp(current_cmd->cmd, "export") == 0)
            {
                handle_export(current_cmd, env_list);
                continue;
            }
        }
        if (tokens && tokens->type == TYPE_PIPE)
            tokens = tokens->next;
    }
    //print_cmd_list(cmd_list);
    return cmd_list;
}

void print_cmd_list(t_cmd_list *cmd_list)
{
    while (cmd_list)
    {
        printf("Commande : %s\n", cmd_list->cmd);
        printf("Arguments de la commande : \n");
        for (int i = 0; cmd_list->cmd_args && cmd_list->cmd_args[i]; i++)
        {
            printf("cmd_args[%d] = %s\n", i, cmd_list->cmd_args[i]);
        }
        printf("Liste des fichiers (infile et outfile) : \n");
        for (int i = 0; i < cmd_list->files_count; i++)
        {
            const char *type_str = "";
            if (cmd_list->files_type[i] == 0)
                type_str = "infile";
            else if (cmd_list->files_type[i] == 1)
                type_str = "outfile (overwrite)";
            else if (cmd_list->files_type[i] == 2)
                type_str = "outfile (append)";
            printf("files_list[%d] = %s (type = %d : %s)\n", i, cmd_list->files_list[i], cmd_list->files_type[i], type_str);
        }
        if (cmd_list->last_in != -1)
            printf("last_in = files_list[%d]\n", cmd_list->last_in);
        else
            printf("last_in = -1\n");

        if (cmd_list->last_out != -1)
            printf("last_out = files_list[%d]\n", cmd_list->last_out);
        else
            printf("last_out = -1\n");
        cmd_list = cmd_list->next;
        printf("\n");
    }
}

void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_list *tmp;

    while (cmd_list)
    {
        tmp = cmd_list;
        cmd_list = cmd_list->next;

        if (tmp->cmd_args)
        {
            for (int i = 0; tmp->cmd_args[i]; i++)
            {
                free(tmp->cmd_args[i]);
            }
            free(tmp->cmd_args);
        }

        if (tmp->cmd)
        {
            free(tmp->cmd);
        }

        if (tmp->files_list)
        {
            for (int i = 0; i < tmp->files_count; i++)
            {
                free(tmp->files_list[i]);
            }
            free(tmp->files_list);
        }

        if (tmp->files_type)
        {
            free(tmp->files_type);
        }
        free(tmp);
    }
}
