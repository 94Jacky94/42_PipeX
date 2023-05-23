/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_parsing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/23 02:49:38 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"
#define LEN_PATH 5
#define LEN_HERE_DOC 8

char	try_access(char *cmd_arg, char ***arg)
{
	int	i;

	if (*arg)
	{
		i = -1;
		while (*((*arg) + (++i)))
			free(*((*arg) + i));
		free(*arg);
	}
	*arg = ft_split(cmd_arg, ' ');
	if (*arg == NULL || **arg == 0)
		return (-1);
	if ((***arg == '.' && *((**arg) + 1) == '/') || ***arg == '/')
	{
		if (access(**arg, F_OK | X_OK) == -1)
			return (-1);
	}
	else
		return (-1);
	return (0);
}

char	*search_folders(char **env)
{
	char	*str;
	int		i;

	str = "PATH=";
	while (*env)
	{
		i = -1;
		while (++i < LEN_PATH)
			if (*(*env + i) != *(str + i))
				break ;
		if (i == 5)
			return (*env);
		++env;
	}
	return (NULL);
}

char	try_path(char *env_var, char **path, int i)
{
	char			test;
	char			*new;

	test = 0;
	if (env_var == NULL)
		return (-1);
	while (*(env_var + i))
	{
		new = ft_strjoin(env_var, *path, &i);
		if (new == NULL)
			return (-1);
		if (access(new, F_OK | X_OK) != -1)
		{
			test = 1;
			break ;
		}
		if (new)
			free(new);
	}
	if (test == 1)
		return (free(*path), *path = new, 0);
	return (-1);
}
