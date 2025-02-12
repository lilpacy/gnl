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

static char	*process_line_with_newline(char **saved, char *newline_pos)
{
	char	*line;
	char	*temp;
	size_t	len;

	len = newline_pos - *saved + 1;
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
	{
		free(*saved);
		*saved = NULL;
		return (NULL);
	}
	ft_strlcpy(line, *saved, len + 1);
	line[len - 1] = '\n';
	line[len] = '\0';
	temp = ft_strdup(newline_pos + 1);
	free(*saved);
	if (!temp)
	{
		free(line);
		*saved = NULL;
		return (NULL);
	}
	*saved = temp;
	return (line);
}

static char	*extract_line(char **saved)
{
	char	*line;
	char	*newline_pos;

	if (!*saved || !**saved)
	{
		if (*saved)
		{
			free(*saved);
			*saved = NULL;
		}
		return (NULL);
	}
	newline_pos = ft_strchr(*saved, '\n');
	if (newline_pos)
		return (process_line_with_newline(saved, newline_pos));
	line = ft_strdup(*saved);
	free(*saved);
	*saved = NULL;
	if (!line)
		return (NULL);
	return (line);
}

static char	*process_buffer(char **saved, char *buffer)
{
	char	*temp;

	if (*saved)
		temp = ft_strjoin(*saved, buffer);
	else
		temp = ft_strdup(buffer);
	if (!temp)
	{
		free(*saved);
		*saved = NULL;
		return (NULL);
	}
	free(*saved);
	*saved = temp;
	return (*saved);
}

static char	*read_to_saved(int fd, char **saved)
{
	char	*buffer;
	ssize_t	read_bytes;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (read_bytes > 0 && (!*saved || !ft_strchr(*saved, '\n')))
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buffer);
			free(*saved);
			*saved = NULL;
			return (NULL);
		}
		if (read_bytes == 0)
			break ;
		buffer[read_bytes] = '\0';
		if (!process_buffer(saved, buffer))
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (*saved);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!saved || !ft_strchr(saved, '\n'))
	{
		if (!read_to_saved(fd, &saved))
			return (NULL);
	}
	line = extract_line(&saved);
	if (!line || !*line)
	{
		free(line);
		if (saved)
		{
			free(saved);
			saved = NULL;
		}
		return (NULL);
	}
	return (line);
}
