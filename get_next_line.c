/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 11:32:34 by tpouget           #+#    #+#             */
/*   Updated: 2020/08/09 00:43:41 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	get_line_from(char **buffer, char **line)
{
	char		*end_of_line;
	char		*old_line;
	char		*old_buffer;

	if (!*buffer)
	{
		*buffer = malloc(BUFFER_SIZE + 1);
		return (*buffer ? 1 : -1);
	}
	old_line = *line;
	old_buffer = *buffer;
	if ((end_of_line = ft_strchr(*buffer, '\n')))
	{
		*end_of_line = '\0';
		*line = ft_strjoin(*line, *buffer);
		*buffer = ft_strdup(end_of_line + 1);
		free(old_buffer);
	}
	else
		*line = ft_strjoin(*line, *buffer);
	free(old_line);
	if (!*line || !*buffer)
		return (-1);
	return (end_of_line ? 0 : 1);
}

int			get_next_line(int fd, char **line)
{
	static char	*buffers[OPEN_MAX];
	ssize_t		bytes_read;
	int			ret;
	char		*old_buffer;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (-1);
	if(!buffers[fd] && !(bytes_read = read(fd, buffers[fd], BUFFER_SIZE)))
		return (0);
	while ((ret = get_line_from(&buffers[fd], line)))
	{
		old_buffer = buffers[fd];
		if (ret < 0 || !(buffers[fd] = malloc(BUFFER_SIZE + 1)))
			return (-1);
		free(old_buffer);
		bytes_read = read(fd, buffers[fd], BUFFER_SIZE);
		if (bytes_read < 0)
			return (-1);
		else if (!bytes_read)
		{
			free(buffers[fd]);
			buffers[fd] = NULL;
			return (1);
		}
		else
			buffers[fd][bytes_read] = '\0';
	}
	return (1);
}
