/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:08:58 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/11 20:55:49 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

int g_last_exit_status = 0;
int g_status = 0;
volatile sig_atomic_t g_received_signal = 0;

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

// Fonction pour configurer le mode du terminal
void set_terminal_mode(int canonical)
{
    struct termios term;
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
        // Si interruption, on affiche une invite vide et ne redessine pas
        input = readline("");
        g_received_signal = 0;  // Réinitialise après interruption
    }
    else
    {
        input = readline("minishell$ ");
    }

    if (input && *input)
        add_history(input);

    return input;
}


void    process_input(char *input, t_data *data, t_env **env_list)
{
    t_token     *tokens;
    t_cmd_list  *cmd_list;

    tokens = tokenize_input(input, *env_list);
    free(input);

    if (check_syntax(tokens) == 0)
    {
        cmd_list = parse_commands(tokens, env_list);
        if (cmd_list)
        {
            (void)data;
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
		if (g_status == 1)
		{
			display_prompt();
			continue;
		}

		input = get_user_input();

		if (!input)
		{
			write(1, "exit\n", 5);
			break;
		}

		process_input(input, &data, &env_list);
	}

	ft_freetab(data.av);
	ft_freetab(data.envp);
	free(data.cwd);

	return (0);
}

