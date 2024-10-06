#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

int	g_last_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_data	data;


	data.ac = argc;
	data.av = argv;
	data.envp = ft_copytab(envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize_input(input);
		// printf("Tokens générés : \n");
		// print_tokens(tokens);
		if (check_syntax(tokens) == 0)
			ft_exec(tokens, &data);
		free_tokens(tokens);
		free(input);
		
	}
	ft_freedata(&data);
	printf("exit\n");
	return (0);
}
