/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 18:39:37 by jboyreau          #+#    #+#             */
/*   Updated: 2022/12/04 18:28:42 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

int	ft_strlen(char *str, char mode)
{
	int	i;

	if (str == 0)
		return (0);
	i = 0;
	if (mode == 1)
	{
		while (*(str + i) != '\n' && *(str + i) && i < BUFFER_SIZE)
			i++;
		return (i);
	}
	while (*(str + i))
		i++;
	return (i);
}

char	ft_check_next_line(char *next_line, int test)
{
	int	i;

	i = 0;
	while (*(next_line + i) != '\n' && *(next_line + i) != 0)
		i++;
	if (*(next_line + i) == '\n' || test == 0)
		return (0);
	return (1);
}

char	*ft_reshape(char *next_line)
{
	char	*tmp;
	int		i;

	tmp = malloc(ft_strlen(next_line, 2) + 1);
	if (tmp == 0)
	{
		if (next_line)
			free(next_line);
		return (0);
	}
	i = -1;
	while (*(next_line + (++i)))
		*(tmp + i) = *(next_line + i);
	free(next_line);
	*(tmp + i) = 0;
	return (tmp);
}
