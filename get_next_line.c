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
		return (NULL);
	ft_strlcpy(line, *saved, len + 1);
	line[len - 1] = '\n';
	line[len] = '\0';
	temp = ft_strdup(newline_pos + 1);
	if (!temp)
	{
		free(line);
		return (NULL);
	}
	free(*saved);
	*saved = temp;
	return (line);
}

static char	*extract_line(char **saved)
{
	char	*line;
	char	*newline_pos;

	if (!*saved || !**saved)
		return (NULL);
	newline_pos = ft_strchr(*saved, '\n');
	if (newline_pos)
		return (process_line_with_newline(saved, newline_pos));
	line = ft_strdup(*saved);
	free(*saved);
	*saved = NULL;
	return (line);
}

static char	*process_buffer(char **saved, char *buffer, ssize_t read_bytes)
{
	char	*temp;

	buffer[read_bytes] = '\0';
	if (*saved)
		temp = ft_strjoin(*saved, buffer);
	else
		temp = ft_strjoin("", buffer);
	if (!temp)
		return (NULL);
	free(*saved);
	*saved = temp;
	return (*saved);
}

static char	*allocate_buffer(char **saved)
{
	char	*buffer;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		free(*saved);
		*saved = NULL;
		return (NULL);
	}
	return (buffer);
}

static int	handle_read_error(char **saved, char *buffer)
{
	free(buffer);
	free(*saved);
	*saved = NULL;
	return (0);
}

static int	read_chunk(int fd, char **saved, char *buffer)
{
	ssize_t	read_bytes;

	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes == -1)
		return (handle_read_error(saved, buffer));
	if (!process_buffer(saved, buffer, read_bytes))
		return (handle_read_error(saved, buffer));
	return (read_bytes);
}

static char	*read_to_saved(int fd, char **saved)
{
	char	*buffer;
	ssize_t	read_bytes;

	buffer = allocate_buffer(saved);
	if (!buffer)
		return (NULL);
	read_bytes = 1;
	while (read_bytes > 0 && (!*saved || !ft_strchr(*saved, '\n')))
	{
		read_bytes = read_chunk(fd, saved, buffer);
		if (read_bytes <= 0)
		{
			free(buffer);
			if (read_bytes == 0)
				return (*saved);
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
		{
			if (saved)
			{
				line = extract_line(&saved);
				if (!line)
				{
					free(saved);
					saved = NULL;
				}
				return (line);
			}
			return (NULL);
		}
	}
	return (extract_line(&saved));
}
