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



int main(int argc, char **argv, char **envp)
{
    char        *input;
    t_token     *tokens;
    t_cmd_list  *cmd_list;
    t_data      data;

    data.ac = argc;
    data.av = ft_copytab(argv);
    data.envp = ft_copytab(envp);
    data.cwd = malloc(4096);
    data.exit = 0;
    getcwd(data.cwd, 1024);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

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
            cmd_list = parse_commands(tokens);
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
