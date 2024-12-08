/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:42:02 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 23:42:59 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

void	configure_terminal(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

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
	(void)signum;
	g_received_signal = 131;
	rl_replace_line("", 0);
	rl_redisplay();
}
