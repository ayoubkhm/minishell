#include "signals.h"
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

void	display_prompt(void)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler(int signum)
{
	struct termios	term;

	g_received_signal = signum;
	// printf("g received signal = %d\n", g_received_signal);
	write(1, "\n", 1);
	tcgetattr(STDIN_FILENO, &term);
	if (!(term.c_lflag & ICANON))
	{
	}
	else
	{
		display_prompt();
	}
}

void	sigquit_handler(int signum)
{
	g_received_signal = 131;
	printf("signum = %d\n", signum);
	write(1, "Quit\n", 5);
	rl_replace_line("", 0);
	rl_redisplay();
}