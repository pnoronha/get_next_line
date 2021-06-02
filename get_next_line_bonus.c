/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoronha <pnoronha@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 09:58:55 by pnoronha          #+#    #+#             */
/*   Updated: 2021/06/01 14:13:41 by pnoronha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	end_file(char **stack, char **line)
{
	int	i;

	i = 0;
	*line = ft_strdup(stack[i]);
	free(stack[i]);
	stack[i] = NULL;
}

static int	put_line(char **stack, char **line)
{
	char	*tmp;
	int		i;

	i = 0;
	while ((*stack)[i] != '\0' && (*stack)[i] != '\n')
		i++;
	if (ft_strchr(*stack, '\n'))
	{
		*line = ft_substr(*stack, 0, i);
		tmp = ft_strdup(ft_strchr(*stack, '\n') + 1);
		free(*stack);
		*stack = tmp;
		return (1);
	}
	return (0);
}

static void	fill_stack(char **stack, char *buffer)
{
	char	*tmp;

	tmp = NULL;
	if (!*(stack))
	{
		*stack = ft_strdup(buffer);
		return ;
	}
	tmp = *stack;
	*stack = ft_strjoin(tmp, buffer);
	free(tmp);
}

int	get_next_line(int fd, char **line)
{
	static char	*stack[4096];
	char		buffer[BUFFER_SIZE + 1];
	int			read_ret;

	if (!line || fd < 0 || read(fd, 0, 0) == -1 || BUFFER_SIZE < 1)
		return (-1);
	if (stack[fd] && put_line(&stack[fd], line))
		return (1);
	read_ret = read(fd, buffer, BUFFER_SIZE);
	while (read_ret > 0)
	{
		buffer[read_ret] = '\0';
		fill_stack(&stack[fd], buffer);
		if (put_line(&stack[fd], line))
			return (1);
		read_ret = read(fd, buffer, BUFFER_SIZE);
	}
	if (stack[fd] && !ft_strchr(stack[fd], '\n'))
	{
		end_file(&stack[fd], line);
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}
