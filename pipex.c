/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/04 15:36:58 by jboyreau         ###   ########.fr       */
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

	if (argc < 3)
		return (-1);
	str = "here_doc";
	j = -1;
	while (++j < LEN_HERE_DOC)
		if (*(*(argv + 1) + j) != *(str + j))
			break ;
	if (j != 8)
	{
		if (open_files(fd, argc, argv) == -1)
			return (-1);
		return (1);
	}
	if (j == 8)
	{
		if (open_files2(fd, argc, argv) == -1)
			return (-1);
		++(*i);
		return (2);
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
	while (++i < limit)
		waitpid(*(fout + i), &status, 0);
}

void	destroy(char **arg, int *fd, pid_t *fout)
{
	int	i;

	i = -1;
	while (++i < 4)
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
}

void	err(char **arg, char mode)
{
	int	i;

	if (mode == 1)
	{
		i = 0;
		while (*(*arg + i))
			++i;
		write(2, *arg, i);
		write(2, " : unfound\n", 11);
		return ;
	}
	write (2, "transfert fail\n", 16);
}

int	main(int argc, char **argv, char **env)
{
	static t_mi		m = {.i = 1, .mode = 0};
	static int		inc = -1;
	static int		fd[5];
	static char		**arg;
	static pid_t	*fout;

	m.mode = check(fd, argv, &(m.i), argc);
	if (m.mode == -1)
		return (destroy(arg, fd, fout), EXIT_FAILURE);
	fout = malloc((argc - 3) * sizeof(pid_t));
	if (fout == NULL)
		return (-1);
	while (++(m.i) <= *(fd + 4))
	{
		if (try_access(*(argv + m.i), &arg) == -1)
			if (try_path(search_folders(env), arg, PATH_START) == -1)
				return (err(arg, 1), destroy(arg, fd, fout), EXIT_FAILURE);
		*(fout + (++inc)) = transfer(arg, env, fd, m);
		if (*(fout + inc) == -1)
			return (err(arg, 2), destroy(arg, fd, fout), EXIT_FAILURE);
	}
	return (wait_cmd(fout, argc, m.mode), destroy(arg, fd, fout), EXIT_SUCCESS);
}
