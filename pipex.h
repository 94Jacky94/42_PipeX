/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jboyreau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:06:58 by jboyreau          #+#    #+#             */
/*   Updated: 2023/05/04 15:36:58 by jboyreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct mode_index
{
	int		i;
	char	mode;
}		t_mi;

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2, int *i);
char	try_access(char *cmd_arg, char ***arg);
char	*search_folders(char **env);
char	try_path(char *env_var, char **path, int i);
char	open_files(int *fd, int argc);
char	open_files2(int *fd, int argc);
pid_t	transfer(char **arg, char **env, int *fd, t_mi mi);

#endif
