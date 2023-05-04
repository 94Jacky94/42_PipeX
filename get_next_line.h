/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 18:22:11 by jboyreau          #+#    #+#             */
/*   Updated: 2022/12/04 18:29:32 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 80
# endif
# if (BUFFER_SIZE < 0)
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 0
# endif
# define FMA 1023
# define FMI 0

int		ft_strlen(char *str, char mode);
char	*get_next_line(int fd);
char	ft_check_next_line(char *next_line, int test);
char	*ft_reshape(char *next_line);
#endif
