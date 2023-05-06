/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/06 02:23:17 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"
#define STDIN 0
#define LEN_FD 4

static void	fill_pipe(int *fd2, char *str)
{
	char	*buffer;
	int		inc;
	int		j;

	while (1)
	{
		buffer = gnl(STDIN);
		if (buffer == NULL)
			return (write(2, "gnl_stdin_failed\n", 17), (void)0);
		inc = 0;
		while (*(buffer + inc))
			++inc;
		j = -1;
		while (*(str + (++j)))
			if (*(buffer + j) != *(str + j))
				break ;
		if (*(buffer + j) == '\n' && *(str + j) == '\0')
			return (free(buffer), (void)0);
		write(*(fd2 + 1), buffer, inc);
		free(buffer);
	}
	return ;
}

static char	define_in_out2(t_fds f, t_mi m, char **argv)
{
	if (m.i == 3 && m.mode == 2)
	{
		if (pipe(f.fd2) == -1)
			return (-1);
		fill_pipe(f.fd2, *(argv + 2));
		if (dup2(*(f.fd2), 0) == -1)
			return (-1);
		return (close(*(f.fd2)), close(*(f.fd2 + 1)), 0);
	}
	if (m.mode == 0)
	{
		if (pipe(f.fd2) == -1)
			return (-1);
		if (dup2(*(f.fd2), 0) == -1)
			return (-1);
		return (close(*(f.fd2)), close(*(f.fd2 + 1)), 0);
	}
	if (dup2(*(f.fd + 2), 0) == -1)
		return (-1);
	return (0);
}

static char	define_in_out(t_fds f, char **argv, t_mi m)
{
	if (pipe(f.fd) == -1 && (*(f.fd + 4)) != m.i)
		return (-1);
	if (m.i == 2 && (m.mode == 1 || m.mode == 0))
	{
		define_in_out2(f, m, argv);
		if (dup2(*(f.fd + 1), 1) == -1)
			return (-1);
		return (0);
	}
	if (m.i == (*(f.fd + 4)))
	{
		if (dup2(*(f.fd + 3), 1) == -1)
			return (-1);
		return (0);
	}
	if (define_in_out2(f, m, argv) == -1)
		return (-1);
	if (dup2(*(f.fd + 1), 1) == -1)
		return (-1);
	return (0);
}

pid_t	transfer(char **arg, t_tabs t, int *fd, t_mi mi)
{
	pid_t			fout;
	static int		fd2[2];
	t_fds			f;

	f.fd = fd;
	f.fd2 = fd2;
	if (define_in_out(f, t.argv, mi) == -1)
		return (-1);
	fout = fork();
	if (fout > 0)
	{
		if (dup2(*(f.fd), 0))
			return (-1);
		return (close(*(f.fd)), close(*(fd + 1)), fout);
	}
	fout = -1;
	while (++fout < LEN_FD)
		if (*(fd + fout) > 1)
			close(*(fd + fout));
	if (arg != NULL)
		if (*arg != NULL)
			execve(*arg, arg, t.env);
	return (-2);
}

char	open_files(int *fd, int argc, char **argv, char mode)
{
	if (mode == 1)
	{
		*(fd + 2) = open(*(argv + 1), O_RDONLY);
		*(fd + 3) = open(*(argv + argc - 1), O_WRONLY | O_TRUNC | O_CREAT, 0664
				);
		if ((*(fd + 3) == -1) && (*(fd + 2) > -1))
			return (write(2, "Ouput unfound\n", 14), close(*(fd + 2)), -1);
		else if (*(fd + 3) == -1)
			return (write(2, "I/O unfound\n", 12), -1);
		if (*(fd + 2) == -1)
			return (*(fd + 4) = argc - 2, write(2, "Input unfound\n", 14), 1);
		*(fd + 4) = argc - 2;
		return (0);
	}
	*(fd + 3) = open(*(argv + argc - 1), O_WRONLY | O_APPEND | O_CREAT, 0664);
	if (*(fd + 3) == -1)
		return (-1);
	*(fd + 4) = argc - 2;
	return (0);
}
