#include "parsing.h"

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: %s   Type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}
