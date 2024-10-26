#include "parsing.h"

t_token *tokenize_input(char *input)
{
    int i = 0;
    t_token *tokens = NULL;

    while (input[i])
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        if (input[i] == '\'')
        {
            i = handle_single_quotes(input, i, &tokens);  // Mettre à jour i correctement après la gestion des quotes simples
        }
        else if (input[i] == '"')
        {
            i = handle_double_quotes(input, i, &tokens);  // Mettre à jour i correctement après la gestion des quotes doubles
        }
        else if (input[i] == '$')
        {
            // Hors des quotes, on ajoute le token directement
            handle_variable_expansion(input, &i, 0, &tokens);
        }
        else if (is_operator(input[i]))
        {
            i = handle_operator(input, i, &tokens);
        }
        else 
        {
            i = handle_word(input, i, &tokens);
        }
    }
    return tokens;
}



int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int handle_operator(char *input, int i, t_token **tokens)
{
    if (input[i] == '>' && input[i + 1] == '>')
    {
        // Redirection en mode append (>>)
        add_token(tokens, create_token(">>", TYPE_REDIRECTION_APPEND));
        return i + 2;
    }
    else if (input[i] == '>')
    {
        // Redirection de sortie (>)
        add_token(tokens, create_token(">", TYPE_REDIRECTION_OUTPUT));
        return i + 1;
    }
    else if (input[i] == '<' && input[i + 1] == '<')
    {
        // Here-document (<<)
        add_token(tokens, create_token("<<", TYPE_HEREDOC));
        return i + 2;
    }
    else if (input[i] == '<')
    {
        // Redirection d'entrée (<)
        add_token(tokens, create_token("<", TYPE_REDIRECTION_INPUT));
        return i + 1;
    }
    else if (input[i] == '|')
    {
        // Pipe (|)
        add_token(tokens, create_token("|", TYPE_PIPE));
        return i + 1;
    }
    return i;
}


int handle_word(char *input, int i, t_token **tokens)
{
    int start = i;
    while (input[i] && !isspace(input[i]) && !is_operator(input[i]) && input[i] != '\'' && input[i] != '"')
        i++;
    char *word = ft_substr(input, start, i - start);
    add_token(tokens, create_token(word, TYPE_WORD));
    free(word);
    return i;
}