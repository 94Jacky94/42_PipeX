/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/07 15:58:35 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"
#define PATH_START 5
#define LEN_HERE_DOC 8

char	check(int *fd, char **argv, int *i, int argc)
{
	int			j;
	static char	*str = "here_doc";

	if (argc < 5)
		return (write(2, "Missing arguments\n", 18), -1);
	j = -1;
	while (++j < LEN_HERE_DOC)
		if (*(*(argv + 1) + j) != *(str + j))
			break ;
	if (j != 8)
	{
		j = open_files(fd, argc, argv, 1);
		if (j == -1)
			return (-1);
		if (j == 1)
			return (++(*i), 0);
		return (1);
	}
	if (argc < 6)
		return (write(2, "Missing arguments\n", 18), -1);
	if (open_files(fd, argc, argv, 2) == -1)
		return (-1);
	return (++(*i), 2);
}

void	wait_cmd(int argc, char mode)
{
	int	status;
	int	limit;

	if (mode == 1)
		limit = argc - 3;
	else if (mode == 0)
		limit = argc - 4;
	else
		limit = argc - 4;
	while (--limit > -1)
		waitpid(-1, &status, 0);
}

void	destroy(char **arg, int *fd, int *fd_org, pid_t *fout)
{
	int	i;

	i = -1;
	while (++i < 4)
		if (*(fd + i) > -1)
			close(*(fd + i));
	if (arg)
	{
		i = -1;
		while (*(arg + (++i)))
			if (*(arg + i))
				free(*(arg + i));
		free(arg);
	}
	if (fout)
		free(fout);
	(dup2(*(fd_org), 0), close(*(fd_org)));
	(dup2(*(fd_org + 1), 1), close(*(fd_org + 1)));
}

void	err(char **arg)
{
	int	i;

	i = 0;
	if (arg)
		if (*arg)
			while (*(*arg + i))
				++i;
	if (arg)
	{
		if (*arg)
			(write(2, *arg, i), write(2, " : Command not found\n", 21));
		else
			write(2, "Command not found\n", 18);
	}
	else
		write(2, "Command not found\n", 18);
}

int	main(int argc, char **argv, char **env)
{
	static t_mi		m = {.i = 1, .j = -1, .mode = 0};
	static t_tabs	t;
	static int		fd[5];
	static pid_t	*fout;

	t.argv = argv;
	t.env = env;
	*(t.fd_org) = dup(0);
	*(t.fd_org + 1) = dup(1);
	m.mode = check(fd, argv, &(m.i), argc);
	if (m.mode == -1)
		return (destroy(t.arg, fd, t.fd_org, fout), EXIT_FAILURE);
	fout = malloc((argc - 3) * sizeof(pid_t));
	if (fout == NULL)
		return (EXIT_FAILURE);
	while (++(m.i) <= *(fd + 4))
	{
		if (try_access(*(argv + m.i), &(t.arg)) == -1)
			if (try_path(search_folders(env), t.arg, PATH_START) == -1)
				err(t.arg);
		*(fout + (++(m.j))) = transfer(t.arg, t, fd, m);
		if (*(fout + m.j) == -1)
			return (destroy(t.arg, fd, t.fd_org, fout), EXIT_FAILURE);
	}
	return (wait_cmd(argc, m.mode), destroy(t.arg, fd, t.fd_org, fout), 0);
}
