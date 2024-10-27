/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:08:58 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 13:28:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_exit_status = 0;

void	init_data(int argc, char **argv, char **envp, t_data *data)
{
	data->ac = argc;
	data->av = ft_copytab(argv);
	data->envp = ft_copytab(envp);
	data->cwd = malloc(4096);
	data->exit = 0;
	getcwd(data->cwd, 1024);
}

void	init_signals_and_env(t_env **env_list, char **envp)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	*env_list = init_env(envp);
}

char	*get_user_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (input && *input)
		add_history(input);
	return (input);
}

void	process_input(char *input, t_data *data, t_env **env_list)
{
	t_token		*tokens;
	t_cmd_list	*cmd_list;

	tokens = tokenize_input(input);
	free(input);
	if (check_syntax(tokens) == 0)
	{
		cmd_list = parse_commands(tokens, env_list);
		if (cmd_list)
		{
			ft_exec(cmd_list, data);
			free_cmd_list(cmd_list);
		}
	}
	free_tokens(tokens);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	t_env	*env_list;

	init_data(argc, argv, envp, &data);
	init_signals_and_env(&env_list, envp);
	while (1)
	{
		input = get_user_input();
		if (!input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		process_input(input, &data, &env_list);
	}
	ft_freetab(data.av);
	ft_freetab(data.envp);
	free(data.cwd);
	return (0);
}
