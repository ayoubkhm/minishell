#include "parsing.h"

t_cmd_list	*create_cmd_node(void)
{
	t_cmd_list	*new_node;

	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
	new_node->save_std[0] = -1;
	new_node->save_std[1] = -1;
	new_node->cmd_args = NULL;
	new_node->cmd = NULL;
	new_node->files_list = NULL;
	new_node->files_type = NULL;
	new_node->files_count = 0;
	new_node->last_in = -1;
	new_node->last_out = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_cmd_list	*init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd)
{
	t_cmd_list	*new_node;

	new_node = NULL;
	if (!(*cmd_list))
	{
		new_node = create_cmd_node();
		*cmd_list = new_node;
		*current_cmd = new_node;
	}
	else
	{
		new_node = create_cmd_node();
		(*current_cmd)->next = new_node;
		new_node->prev = *current_cmd;
		*current_cmd = new_node;
	}
	new_node->last_in = -1;
	new_node->last_out = -1;
	return (new_node);
}

int	allocate_command_args(t_cmd_list *current_cmd, int arg_count)
{
	int		i;
	char	**cmd_args;

	cmd_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd_args)
		return (-1);
	i = 0;
	while (i <= arg_count)
	{
		cmd_args[i] = NULL;
		i++;
	}
	current_cmd->cmd_args = cmd_args;
	return (0);
}

int	count_non_empty_args(char **cmd_args)
{
	int	arg_count;
	int	non_empty_count;

	arg_count = 0;
	non_empty_count = 0;
	while (cmd_args[arg_count])
	{
		if (cmd_args[arg_count][0] != '\0')
			non_empty_count++;
		arg_count++;
	}
	return (non_empty_count);
}

char	**allocate_new_args(int non_empty_count)
{
	char	**new_args;

	new_args = malloc(sizeof(char *) * (non_empty_count + 1));
	if (!new_args)
		return (NULL);
	return (new_args);
}

void	trans_non_empty_args(char **src_args, char **dest_args, int total_cnt)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < total_cnt)
	{
		if (src_args[i][0] != '\0')
		{
			dest_args[j] = src_args[i];
			j++;
		}
		else
		{
			free(src_args[i]);
		}
		i++;
	}
	dest_args[j] = NULL;
}

void	remove_empty_arguments(t_cmd_list *current_cmd)
{
	int		arg_count;
	int		new_arg_count;
	char	**new_cmd_args;

	if (!current_cmd || !current_cmd->cmd_args)
		return ;
	arg_count = 0;
	while (current_cmd->cmd_args[arg_count])
		arg_count++;
	new_arg_count = count_non_empty_args(current_cmd->cmd_args);
	if (new_arg_count == arg_count)
		return ;
	new_cmd_args = allocate_new_args(new_arg_count);
	if (!new_cmd_args)
		return ;
	trans_non_empty_args(current_cmd->cmd_args, new_cmd_args, arg_count);
	free(current_cmd->cmd_args);
	current_cmd->cmd_args = new_cmd_args;
}

void	post_process_command(t_cmd_list *current_cmd, t_env **env_list)
{
	if (current_cmd->cmd_args && current_cmd->cmd_args[0])
	{
		if (current_cmd->cmd)
		{
			free(current_cmd->cmd);
			current_cmd->cmd = NULL;
		}
		current_cmd->cmd = strdup(current_cmd->cmd_args[0]);
		if (strcmp(current_cmd->cmd, "export") == 0)
		{
			handle_export(current_cmd, env_list);
		}
	}
	remove_empty_arguments(current_cmd);
}

int	init_alloc_cmd(t_token *tokens, t_cmd_list **cmd_lis, t_cmd_list **cur_cmd)
{
	int	arg_count;

	*cur_cmd = init_command_node(cmd_lis, cur_cmd);
	arg_count = count_arguments(tokens);
	if (allocate_command_args(*cur_cmd, arg_count) == -1)
		return (-1);
	return (0);
}

int	proc_command_tokens(t_token **tokens, t_cmd_list *curr_cmd, t_env *env_list)
{
	int	ret;

	ret = process_token_cmd(tokens, curr_cmd, env_list);
	if (ret == -1)
		return (-1);
	post_process_command(curr_cmd, &env_list);
	return (0);
}

void	handle_pipeline(t_token **tokens)
{
	if (*tokens && (*tokens)->type == TYPE_PIPE)
		*tokens = (*tokens)->next;
}

t_cmd_list	*parse_commands(t_token *tokens, t_env **env_list)
{
	t_cmd_list	*cmd_list;
	t_cmd_list	*current_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (init_alloc_cmd(tokens, &cmd_list, &current_cmd) == -1)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (proc_command_tokens(&tokens, current_cmd, *env_list) == -1)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		handle_pipeline(&tokens);
	}
	return (cmd_list);
}
