#include "parsing.h"
#include <readline/readline.h>
#include <readline/history.h>

int g_last_exit_status = 0;

int main(void)
{
    char *input;
    t_token *tokens;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (*input)
            add_history(input);

        tokens = tokenize_input(input);

        // printf("Tokens générés : \n");
        print_tokens(tokens);

        if (check_syntax(tokens) == 0)
        {
        }
        free_tokens(tokens);
        free(input);
    }
    printf("exit\n");
    return 0;
}

