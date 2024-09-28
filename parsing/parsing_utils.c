#include "parsing.h"

t_token *create_token(char *value, int type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return NULL;
    new->value = ft_strdup(value);
    new->type = type;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *last;

    if (!*tokens)
    {
        *tokens = new_token;
    }
    else
    {
        last = *tokens;
        while (last->next)
            last = last->next;
        last->next = new_token;
        new_token->prev = last; // Initialiser le pointeur prev
    }
}

