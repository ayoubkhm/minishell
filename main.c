/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:23:55 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/09 17:51:21 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

void	init_data(int argc, char **argv, char **envp, t_data *data)
{
	data->ac = argc;
	data->av = ft_copytab(argv);
	if (!data->av)
		ft_freetab(data->av);
	data->envp = ft_updateshlvl(envp);
	data->cwd = malloc(4096);
	data->exit = 0;
	data->nodenb = 0;
	data->prev_pipe_read_end = -1;
	if (!data->cwd)
	{
		perror("malloc failed in init_data");
		exit(1);
	}
	getcwd(data->cwd, 1024);
	if (!data->cwd)
		free(data->cwd);
}

char	*get_user_input(void)
{
	char	*input;

	if (g_received_signal == SIGINT)
	{
		input = readline("");
		g_received_signal = 0;
	}
	else
	{
		input = readline("minishell$ ");
	}
	if (input && *input)
		add_history(input);
	return (input);
}

void	process_input(char *input, t_data *data, t_env **env_list)
{
	t_token		*tokens;
	t_cmd_list	*cmd_list;

	tokens = tokenize_input(input, *env_list);
	free(input);
	if (check_syntax(tokens) == 0)
	{
		cmd_list = parse_commands(tokens, env_list);
		free_tokens(tokens);
		if (cmd_list && is_dangerous_command(cmd_list, env_list) == 0)
		{
			initpipe(cmd_list);
			ft_exec(cmd_list, data, env_list);
			if (!data->envp)
				cleanup_resources(data, env_list, cmd_list);
			else
				(*env_list)->exit_status = data->exit;
			free_cmd_list(cmd_list);
		}
	}
	else
	{
		(*env_list)->exit_status = 2;
		free_tokens(tokens);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	t_env	*env_list;

	init_data(argc, argv, envp, &data);
	init_signals_and_env(&env_list, data.envp);
	while (1)
	{
		if (g_received_signal == 1)
		{
			display_prompt();
			continue ;
		}
		ft_testsig(env_list);
		input = get_user_input();
		if (!input)
		{
			write(1, "exit\n", 5);
			cleanup_resources(&data, &env_list, NULL);
			break ;
		}
		process_input(input, &data, &env_list);
	}
	return (data.exit);
}
