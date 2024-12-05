#include "signals.h"
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

t_signal_context *sig_status = NULL;
void display_prompt(void)
{
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_status = 0;
}

void sigint_handler(int signum)
{
    g_received_signal = signum;
    g_last_exit_status = 130;
    write(1, "\n", 1);

    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    if (!(term.c_lflag & ICANON))
    {
    }
    else
    {
        display_prompt();
    }
}

void sigquit_handler(int signum)
{
    (void)signum;
    write(1, "Quit\n", 5);
    g_last_exit_status = 131;
    rl_replace_line("", 0);
    rl_redisplay();
}