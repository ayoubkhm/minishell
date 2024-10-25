#include "parsing.h"

int handle_pipe(int i, t_token **tokens)
{
    add_token(tokens, create_token("|", TYPE_PIPE));
    return i + 1;
}
