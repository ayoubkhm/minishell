#include "parsing.h"

int handle_redirection(char *input, int i, t_token **tokens)
{
    if (input[i] == '>' && input[i + 1] == '>')
    {
        add_token(tokens, create_token(">>", TYPE_REDIRECTION_APPEND));
        return i + 2;
    }
    else if (input[i] == '<' && input[i + 1] == '<')
    {
        add_token(tokens, create_token("<<", TYPE_HEREDOC));
        return i + 2;
    }
    else if (input[i] == '>')
    {
        add_token(tokens, create_token(">", TYPE_REDIRECTION_OUTPUT));
        return i + 1;
    }
    else if (input[i] == '<')
    {
        add_token(tokens, create_token("<", TYPE_REDIRECTION_INPUT));
        return i + 1;
    }
    return i;
}
