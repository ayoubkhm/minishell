/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:02:34 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/05 19:21:56 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    write(1, "\b\b  \b\b", 4);  // Ignore `SIGQUIT` proprement

}
