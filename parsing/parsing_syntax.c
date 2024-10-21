#include "parsing.h"

int check_syntax(t_token *tokens)
{
    t_token *current = tokens;

    // Si le premier token est un pipe, erreur de syntaxe
    if (current && current->type == TYPE_PIPE)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token '|'\n");
        return -1;
    }

    while (current)
    {
        // Vérification des pipes mal placés
        if (current->type == TYPE_PIPE)
        {
            printf("Token PIPE détecté.\n");
            // Pipe ne doit pas être le dernier token ou être répété sans commande entre deux pipes
            if (!current->next || current->next->type == TYPE_PIPE)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token '|'\n");
                return -1;
            }
        }

        // Vérification des redirections sans fichier
        if (current->type == TYPE_REDIRECTION_INPUT || current->type == TYPE_REDIRECTION_OUTPUT || current->type == TYPE_REDIRECTION_APPEND)
        {
            // Le token suivant ne doit pas être un opérateur, un pipe ou absent
            if (!current->next || current->next->type != TYPE_WORD)
            {
                fprintf(stderr, "minishell: syntax error near unexpected token '%s'\n", current->value);
                return -1;
            }
        }

        current = current->next;
    }

    // Vérification si le dernier token est un pipe (|) ou une redirection sans fichier
    if (tokens && current && (current->type == TYPE_PIPE || 
                              current->type == TYPE_REDIRECTION_INPUT || 
                              current->type == TYPE_REDIRECTION_OUTPUT || 
                              current->type == TYPE_REDIRECTION_APPEND))
    {
        fprintf(stderr, "minishell: syntax error near unexpected token 'newline'\n");
        return -1;
    }

    return 0;
}
