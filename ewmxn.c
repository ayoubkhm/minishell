#include "exec.h"



int	makeapipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	return (0);
}

void	ft_execute_child(t_cmd_list *list, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_execpipe(list);
	ft_exec1(list);
	ft_exec2(list, data);
	exit(0);
}

void	ft_close_pipe(int *pipe)
{
	close(pipe[0]);
	close(pipe[1]);
}


