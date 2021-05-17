/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoronha <pnoronha@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 09:58:55 by pnoronha          #+#    #+#             */
/*   Updated: 2021/05/17 15:34:15 by pnoronha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#define BUFFER_SIZE 8


int	ft_strlen(const char *str)
{
	int		len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if (!c && s[i] == '\0')
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*new_str;
	size_t	len_s1;
	size_t	len_s2;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new_str = malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (new_str == NULL)
		return (NULL);
	i = -1;
	while (++i < len_s1)
		new_str[i] = s1[i];
	j = -1;
	while (++j < len_s2)
		new_str[i + j] = s2[j];
	new_str[i + j] = '\0';
	return (new_str);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	str_lengh;
	size_t	min_lengh;
	size_t	i;

	if (!s)
		return (NULL);
	str_lengh = ft_strlen(s);
	min_lengh = ft_strlen(&s[start]);
	if (str_lengh <= start || len <= 0)
		return (ft_strdup(""));
	else if (len > min_lengh)
		len = min_lengh;
	sub_str = malloc(sizeof(char) * (len + 1));
	if (!sub_str)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		sub_str[i] = s[start + i];
		i++;
	}
	sub_str[i] = '\0';
	return (sub_str);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	dup = malloc(sizeof(char) * (s_len + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}


static int	put_line(char *stack , char **line)
{
	int	i;
	char	*tmp;

	i = 0;
	while (stack[i] != '\0' && stack[i] != '\n')
		i++;
	if (ft_strchr(stack, '\n'))
	{
		*line = ft_substr(stack, 0, i);
		tmp = ft_strdup(ft_strchr(stack, '\n') + 1);
		//free(stack);
		stack = tmp;
		return (1);
	}
	return (0);
}

static void	fill_stack(char **stack, char *buffer)
{
	char	*tmp;

	if(!(*stack))
	{
		*stack = ft_strdup(buffer);
		return ;
	}
	tmp = *stack;
	*stack = ft_strjoin(tmp, buffer);
	//free(tmp);
	tmp = NULL;
}

static void	end_line(char **stack, char **line)
{
	int	i;

	i = 0;
	*line = ft_strdup(stack[i]);
	free(stack[i]);
	stack[i] = NULL;
}

int	get_next_line(int fd, char **line)
{
	static char	*stack[4096];
	char		buffer[BUFFER_SIZE + 1];
	int			read_ret;

	if (!line || fd < 0 || read(fd, 0, 0) < 0 || BUFFER_SIZE < 1)
		return (-1);
	if (stack[fd] && put_line(stack[fd], line))
		return (1);
	read_ret = read(fd, buffer, BUFFER_SIZE);
	while (read_ret > 0)
	{
		buffer[read_ret] = '\0';
		fill_stack(&stack[fd], buffer);
		if (put_line(stack[fd], line))
			return (1);
		read_ret = read(fd, buffer, BUFFER_SIZE);
	}
	if (stack[fd] && !ft_strchr(stack[fd], '\n'))
	{
		end_line(&stack[fd], line);
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}


int main()
{
	int fd;
	int ret;
	char *str;

	fd = open("/Users/pnoronha/Desktop/inProgress/get_next_line/gnlTester/files/nl", O_RDONLY);
	if (fd == -1)
	{
		printf("Error, cannot open file\n");
		return (1);
	}
	printf("File:\n\n");
	while ((ret = get_next_line(fd, &str)) != 0)
	{
		printf("[%s]\n", str);
		free(str);
	}
	printf("\n\n:End of File\n");
	if (ret == -1)
	{
		printf("Error, cannot close file\n");
		return (1);
	}
	return (0);
}

