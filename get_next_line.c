/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 20:26:12 by jboyreau          #+#    #+#             */
/*   Updated: 2022/12/04 18:29:48 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#define BF BUFFER_SIZE

static void	ft_realloc2(char *tmp, char *b, int i)
{
	int	j;

	if (i < 0)
		i = 0;
	j = 0;
	while (j < BUFFER_SIZE && *(b + j) != '\n' && *(b + j) != 0)
		*(tmp + (i++)) = *(b + (j++));
	if (*(b + j) == '\n')
	{
		*(tmp + (i++)) = '\n';
		*(tmp + i) = 0;
		i = 0;
		while ((++j) < BUFFER_SIZE && *(b + j) != 0)
			*(b + (i++)) = *(b + j);
		*(b + i) = 0;
		return ;
	}
	*(tmp + i) = 0;
	j = 0;
	*b = 0;
	return ;
}

static char	*ft_alloc(int alloc, char *next_line)
{
	char	*tmp;

	tmp = malloc(alloc);
	if (tmp == 0)
	{
		if (next_line)
			free(next_line);
		return (0);
	}
	return (tmp);
}

static char	*ft_realloc(char *next_line, char *b, int *alloc)
{
	int		i;
	char	*tmp;

	i = -1;
	*alloc <<= 3;
	if (((*alloc) >> 3) > (ft_strlen(next_line, 2) + BUFFER_SIZE + 1))
	{
		(*alloc) >>= 3;
		while (*(next_line + (++i)))
			;
		ft_realloc2(next_line, b, i);
		return (next_line);
	}
	tmp = ft_alloc(*alloc, next_line);
	if (tmp == 0)
		return (0);
	if (next_line != 0)
	{
		while (*(next_line + (++i)))
			*(tmp + i) = *(next_line + i);
		*(tmp + i) = 0;
		free(next_line);
	}
	ft_realloc2(tmp, b, i);
	return (tmp);
}

static int	ft_read(char *b, int fd)
{
	int	test;

	if (*b != 0)
		return (1);
	write(1, ">", 1);
	test = read(fd, b, BUFFER_SIZE);
	if (test < BF)
		*(b + test) = 0;
	return (test);
}

char	*gnl(int fd)
{
	static char	b[BUFFER_SIZE];
	char		*next_line;
	int			test;
	int			alloc;

	test = 1;
	if (fd < FMI || fd > FMA || BF == 0)
		return (0);
	alloc = BUFFER_SIZE;
	next_line = 0;
	while (test != 0)
	{
		test = ft_read(b, fd);
		if (test == -1 || (test == 0 && next_line == 0 && *b == 0))
			return (0);
		next_line = ft_realloc(next_line, b, &alloc);
		if (next_line == 0)
			return (0);
		if (ft_check_next_line(next_line, test) == 0)
			break ;
	}
	next_line = ft_reshape(next_line);
	return (next_line);
}
