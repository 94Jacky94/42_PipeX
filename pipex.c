/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/05 20:37:00 by jboyreau         ###   ########.fr       */
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
	int		j;
	char	*str;

	if (argc < 5)
		return (write(2, "missing arguments\n", 18), -1);
	str = "here_doc";
	j = -1;
	while (++j < LEN_HERE_DOC)
		if (*(*(argv + 1) + j) != *(str + j))
			break ;
	if (j != 8)
	{
		if (open_files(fd, argc, argv, 1) == -1)
			return (-1);
		return (1);
	}
	if (j == 8)
	{
		if (argc < 6)
			return (write(2, "missing arguments\n", 18), -1);
		if (open_files(fd, argc, argv, 2) == -1)
			return (-1);
		return (++(*i), 2);
	}
	return (-1);
}

void	wait_cmd(pid_t *fout, int argc, char mode)
{
	int	i;
	int	status;
	int	limit;

	i = -1;
	if (mode == 1)
		limit = argc - 3;
	else
		limit = argc - 4;
	while (--limit > -1)
	{
		if (*(fout + limit))
			waitpid(-1, &status, 0);
fprintf(stderr, "pid = %d, limit = %d\n", *(fout + limit), limit);
	}
}

void	destroy(char **arg, int *fd, pid_t *fout)
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
	close(0);
	close(1);
	close(2);
}

void	err(char **arg, char mode, int out)
{
	int	i;

	if (mode == 1)
	{
		i = 0;
		if (arg)
			if (*arg)
				while (*(*arg + i))
					++i;
		if (arg && out > -1)
		{
			if (*arg)
			{
				write(2, *arg, i);
				write(2, " : unfound\n", 11);
			}
			else if (out > -1)
				write(2, "Error no command detected\n", 26);
		}
		else if (out > -1)
				write(2, "Error no command detected\n", 26);
	}
	if (out > -1)
		write (2, "Transfert fail\n", 16);
}

int	main(int argc, char **argv, char **env)
{
	static t_mi		m = {.i = 1, .mode = 0};
	t_tabs			t;
	static int		inc = -1;
	static int		fd[5];
	static pid_t	*fout;

	t.argv = argv;
	t.env = env;
	t.arg = NULL;
	m.mode = check(fd, argv, &(m.i), argc);
	if (m.mode == -1)
		return (destroy(t.arg, fd, fout), EXIT_FAILURE);
	fout = malloc((argc - 3) * sizeof(pid_t));
	if (fout == NULL)
		return (-1);
	while (++(m.i) <= *(fd + 4))
	{
		if (try_access(*(argv + m.i), &(t.arg)) == -1)
			if (try_path(search_folders(env), t.arg, PATH_START) == -1)
				err(t.arg, 1, 0);
		*(fout + (++inc)) = transfer(t.arg, t, fd, m);
		if (*(fout + inc) < 0)
			return (err(t.arg, 2, *(fout + inc)), destroy(t.arg, fd, fout), EXIT_FAILURE);
	}
	return (wait_cmd(fout, argc, m.mode), destroy(t.arg, fd, fout), 0);
}
