/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transefert.c                                       :+:      :+:    :+:   */
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

static char	define_in_out(int *fd, int i, int old_read, char mode)
{
	if (pipe(fd) == -1)
		return (-1);
	if ((i == 2 && mode == 1) || (i == 3 && mode == 2))
	{
		if (dup2(*(fd + 2), 0) == -1)
			return (-1);
		if (dup2(*(fd + 1), 1) == -1)
			return (-1);
		return (0);
	}
	if (i == (*(fd + 4)))
	{
		if (dup2(old_read + 0, 0) == -1)
			return (-1);
		if (dup2(*(fd + 3), 1) == -1)
			return (-1);
		return (close(old_read), 0);
	}
	if (dup2(old_read, 0) == -1)
		return (-1);
	if (dup2(*(fd + 1), 1) == -1)
		return (-1);
	return (close(old_read), 0);
}

pid_t	transfer(char **arg, char **env, int *fd, t_mi mi)
{
	pid_t		fout;
	static int	old_read;

	if (define_in_out(fd, mi.i, old_read, mi.mode) == -1)
		return (-1);
	fout = fork();
	if (fout == -1)
		return (-1);
	if (fout > 0)
	{
		old_read = *fd;
		close(*(fd + 1));
	}
	else
		if (execve(*arg, arg, env) == -1)
			return (-1);
	return (fout);
}

char	open_files(int *fd, int argc)
{
	*(fd + 2) = open("./infile", O_RDONLY);
	if (*(fd + 2) == -1)
		return (-1);
	*(fd + 3) = open("./outfile", O_WRONLY | O_TRUNC);
	if (*(fd + 3) == -1)
		return (close(*(fd + 2)), -1);
	*(fd + 4) = argc - 2;
	return (0);
}

char	open_files2(int *fd, int argc)
{
	*(fd + 2) = open("./infile", O_RDONLY);
	if (*(fd + 2) == -1)
		return (-1);
	*(fd + 3) = open("./outfile", O_WRONLY | O_APPEND);
	if (*(fd + 3) == -1)
		return (close(*(fd + 2)), -1);
	*(fd + 4) = argc - 2;
	return (0);
}
