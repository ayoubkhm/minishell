#include "minishell.h"
#include <termios.h>

volatile sig_atomic_t	g_received_signal = 0;

void	init_data(int argc, char **argv, char **envp, t_data *data)
{
	data->ac = argc;
	data->av = ft_copytab(argv);
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

void	init_signals_and_env(t_env **env_list, char **envp)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	if (!envp)
		return ;
	*env_list = init_env(envp);
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
	print_tokens(tokens);
	free(input);
	if (check_syntax(tokens) == 0)
	{
		cmd_list = parse_commands(tokens, env_list);
		free_tokens(tokens);
		if (cmd_list)
		{
			// print_commands(cmd_list);
			(void)data;
			initpipe(cmd_list);
			ft_exec(cmd_list, data, env_list);
			if(!data->envp)
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
	init_signals_and_env(&env_list, envp);
	while (1)
	{
		if (g_received_signal == 1)
		{
			display_prompt();
			continue ;
		}
		if (g_received_signal == 2)
		{
			env_list->exit_status = 130;
		}
		if (g_received_signal == 131)
		{
			env_list->exit_status = 131;
			g_received_signal = 0;
		}
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
