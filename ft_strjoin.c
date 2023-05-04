/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2022/11/15 15:36:58 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*(s + i))
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2, int *i)
{
	char			*s;
	int				inc;
	int				j;

	if (s1 == 0 || s2 == 0)
		return (0);
	s = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (s == 0)
		return (0);
	inc = 0;
	while (*(s1 + (*i)) && *(s1 + (*i)) != ':')
	{
		*(s + inc) = *(s1 + (*i));
		++(*i);
		++inc;
	}
	j = inc;
	*(s + j) = '/';
	++j;
	while (*s2)
		*(s + (j++)) = *(s2++);
	*(s + j) = 0;
	if (*(s1 + (*i)) == ':')
		++(*i);
	return (s);
}
