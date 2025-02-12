/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshinaga <hshinaga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:43:20 by hshinaga          #+#    #+#             */
/*   Updated: 2025/02/06 20:17:55 by hshinaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Input: fd = 3, stash = NULL
** Output: "Hello\n" (first line from file)
**         or "Hello\nWo" (if BUFFER_SIZE > line length)
*/
static char	*gnl_read_and_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	read_size;
	char	*tmp;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	read_size = 1;
	while (!gnl_strchr(stash, '\n') && read_size > 0)
	{
		read_size = read(fd, buf, BUFFER_SIZE);
		if (read_size <= 0)
			break ;
		buf[read_size] = '\0';
		tmp = gnl_strjoin(stash, buf);
		free(stash);
		stash = tmp;
	}
	free(buf);
	if (read_size <= 0 && !stash)
		return (NULL);
	return (stash);
}

/*
** Input: stash = "Hello\nWorld"
** Output: "Hello\n"
*/
static char	*gnl_extract_line(char *stash)
{
	size_t	i;
	char	*line;
	size_t	len;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	len = i;
	if (stash[i] == '\n')
		len++;
	line = gnl_strdup(stash);
	if (line)
		line[len] = '\0';
	return (line);
}

/*
** Input: stash = "Hello\nWorld"
** Output: "World"
*/
static char	*gnl_trim_stash(char *stash)
{
	size_t	i;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = gnl_strdup(stash + i + 1);
	free(stash);
	return (new_stash);
}

/*
** Input: fd = 3 (file containing "Hello\nWorld\n")
** Output: "Hello\n" (first call), "World\n" (second call)
*/
char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = gnl_read_and_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = gnl_extract_line(stash);
	stash = gnl_trim_stash(stash);
	if (!line || !*line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
