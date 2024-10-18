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
	data.av = ft_copytab(argv);
	data.envp = ft_copytab(envp);
	data.cwd = malloc(4096);
	data.exit = 0;
	getcwd(data.cwd, 1024);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize_input(input);
		free(input);
		// printf("Tokens générés : \n");
		// print_tokens(tokens);
		if (check_syntax(tokens) == 0)
			ft_exec(tokens, &data);
		free_tokens(tokens);
	}
	return(0);
}
