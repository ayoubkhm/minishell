/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:57:09 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/05 19:22:17 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "../libft/libft.h"
# include "../minishell.h"

extern volatile sig_atomic_t g_received_signal; 

void	sigint_handler(int sig_num);
void	sigquit_handler(int sig_num);
void display_prompt(void);

#endif
