#include "signals.h"
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

void display_prompt(void)
{
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_status = 0; // Réinitialise l'état d'interruption pour le prompt
}

void sigint_handler(int signum)
{
    g_received_signal = signum;
    write(1, "\n", 1);

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    if (!(term.c_lflag & ICANON))
    {
        // Si un processus est en cours, on ne redessine pas le prompt
    }
    else
    {
        display_prompt();  // Affiche le prompt si aucun processus n’est en cours
    }
}

void sigquit_handler(int signum)
{
    (void)signum;
    write(1, "Quit\n", 5);  // Affiche "Quit" suivi d'une nouvelle ligne
    rl_replace_line("", 0); // Efface le buffer en cours
    rl_redisplay();         // Redessine le prompt
}
