#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

int g_last_exit_status = 0;

void sigint_handler(int sig_num)
{
    (void)sig_num;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay(); 
}

void sigquit_handler(int sig_num)
{
    (void)sig_num;
}

char *ft_strndup(const char *s, size_t n)
{
    char *result = malloc(n + 1);
    if (!result)
        return NULL;
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}


t_env_var *init_env(char **envp) {
    t_env_var *env_list = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++) {
        char *equal_sign = strchr(envp[i], '=');
        if (equal_sign) {
            size_t name_len = equal_sign - envp[i];
            char *name = ft_strndup(envp[i], name_len);
            char *value = strdup(equal_sign + 1);

            t_env_var *new_var = malloc(sizeof(t_env_var));
            new_var->name = name;
            new_var->value = value;
            new_var->next = NULL;

            if (!env_list)
                env_list = new_var;
            else
                current->next = new_var;

            current = new_var;
        }
    }
    return env_list;
}


int main(int argc, char **argv, char **envp)
{
    char        *input;
    t_token     *tokens;
    t_cmd_list  *cmd_list;
    t_data      data;
    t_env_var   *env_list;


    data.ac = argc;
    data.av = ft_copytab(argv);
    data.envp = ft_copytab(envp);
    data.cwd = malloc(4096);
    data.exit = 0;
    getcwd(data.cwd, 1024);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

    env_list = init_env(envp);


    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            write(1, "exit\n", 5);
            break;
        }
        if (*input)
            add_history(input);

        tokens = tokenize_input(input);
        free(input);

        //printf("Tokens générés : \n");
        //print_tokens(tokens);
        if (check_syntax(tokens) == 0)
        {
            cmd_list = parse_commands(tokens, &env_list);
            if (!cmd_list)
            {
                free_tokens(tokens);
                continue;
            }
            ft_exec(cmd_list, &data);
            free_cmd_list(cmd_list);
        }
        free_tokens(tokens);
    }
    ft_freetab(data.av);
    ft_freetab(data.envp);
    free(data.cwd);

    return 0;
}
