/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 20:37:47 by jboyreau          #+#    #+#             */
/*   Updated: 2022/11/18 18:02:27 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char	ft_isc(char test, char c)
{
	if (test == c)
		return (1);
	return (0);
}

static char	ft_alloctxt(char **txt, const char *s, char c)
{
	int	word_size;
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && ft_isc(*s, c))
			s++;
		word_size = 0;
		while (*s && ft_isc(*s, c) == 0)
		{
			word_size++;
			s++;
		}
		if (word_size != 0)
		{
			*(txt + i) = malloc(word_size + 1);
			if (*(txt + i) == 0)
				return (0);
			*(*(txt + (i++)) + word_size) = 0;
		}
	}
	return (1);
}

static void	ft_filltxt(char **txt, const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	while (*s && *(txt + i))
	{
		while (*s && ft_isc(*s, c))
			s++;
		j = 0;
		while (*s && ft_isc(*s, c) == 0)
			*(*(txt + i) + (j++)) = *(s++);
		i++;
	}
}

static unsigned int	ft_countwords(char const *s, char c)
{
	int	nb_words;

	nb_words = 0;
	while (*s)
	{
		if (*s && ft_isc(*s, c) == 0)
		{
			nb_words++;
			s++;
		}
		while (*s && ft_isc(*s, c) == 0)
			s++;
		while (*s && ft_isc(*s, c))
			s++;
	}
	return (nb_words);
}

char	**ft_split(char const *s, char c)
{
	char				**txt;
	unsigned int		nb_words;
	unsigned int		i;

	if (s == 0)
		return (0);
	nb_words = ft_countwords(s, c);
	txt = malloc((nb_words + 1) * sizeof(*txt));
	if (txt == 0)
		return (0);
	*(txt + nb_words) = 0;
	if (nb_words > 0)
	{
		if (ft_alloctxt(txt, s, c) == 0)
		{
			i = 0;
			while (*(txt + i))
				free(*(txt + (i++)));
			free(txt);
			return (0);
		}
		ft_filltxt(txt, s, c);
	}
	return (txt);
}
