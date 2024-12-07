/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:33:04 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/07 23:34:37 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_testsig(t_env *env_list)
{
	if (g_received_signal == 2)
		env_list->exit_status = 130;
	if (g_received_signal == 131)
	{
		env_list->exit_status = 131;
		g_received_signal = 0;
	}
}

void	set_terminal_mode(int canonical)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (canonical)
		term.c_lflag |= ICANON | ECHO;
	else
		term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	init_signals_and_env(t_env **env_list, char **envp)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	if (!envp)
		return ;
	*env_list = init_env(envp);
}

void	cleanup_resources(t_data *data, t_env **env_list, t_cmd_list *list)
{
	t_env	*tmp;

	if (data)
		ft_freedata(data);
	if (list)
	{
		free_cmd_list(list);
		list = NULL;
	}
	while (*env_list)
	{
		tmp = *env_list;
		free((*env_list)->name);
		free((*env_list)->value);
		*env_list = (*env_list)->next;
		free(tmp);
	}
}
