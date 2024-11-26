#include "libft.h"

char *ft_strcat(char *dest, const char *src)
{
    int i = 0;
    int dest_len = ft_strlen(dest);

    while (src[i])
    {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0'; // Ajouter le terminateur
    return dest;
}
