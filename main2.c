/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:33:04 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/10 14:39:14 by gtraiman         ###   ########.fr       */
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
	{
		term.c_lflag |= ICANON | ECHO;
		term.c_lflag |= ECHOCTL;
	}
	else
	{
		term.c_lflag &= ~(ICANON | ECHO);
		term.c_lflag &= ~ECHOCTL;
	}
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

int	is_dangerous_command(t_cmd_list *cmd_list, t_env **env_list)
{
	if (!cmd_list || !cmd_list->next)
		return (0);
	if (cmd_list->cmd_args && ft_strcmp(cmd_list->cmd_args[0],
			"./minishell") == 0 && cmd_list->next->cmd_args
		&& ft_strcmp(cmd_list->next->cmd_args[0], "rm") == 0
		&& cmd_list->next->cmd_args[1] && ft_strcmp(cmd_list->next->cmd_args[1],
			"minishell") == 0)
	{
		cleanup_resources(NULL, env_list, cmd_list);
		return (1);
	}
	return (0);
}
