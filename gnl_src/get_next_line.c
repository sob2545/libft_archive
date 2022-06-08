/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sesim <sesim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 11:53:34 by sesim             #+#    #+#             */
/*   Updated: 2022/06/08 09:30:35 by sesim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*reader(int fd, char *bac)
{
	char	*buf;
	ssize_t	r_cnt;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buf == 0)
		return (0);
	r_cnt = 1;
	while (!ft_strchr(bac, '\n') && r_cnt != 0)
	{
		r_cnt = read(fd, buf, BUFFER_SIZE);
		if (r_cnt == -1)
			break ;
		buf[r_cnt] = '\0';
		bac = ft_strjoin(bac, buf);
	}
	free(buf);
	buf = 0;
	return (bac);
}

char	*get_line(char *bac)
{
	char	*line;
	int		len;

	if (bac[0] == 0)
		return (0);
	if (ft_strchr(bac, '\n'))
		len = ft_strchr(bac, '\n') - bac + 2;
	else
		len = ft_strlen(bac) + 1;
	line = malloc(sizeof(char) * len);
	if (line == 0)
		return (0);
	ft_strlcpy(line, bac, len);
	return (line);
}

t_list	*get_node(t_list *head, int fd)
{
	t_list	*node;

	node = head->next;
	while (node)
	{
		if (node->idx == fd)
			return (node);
		else
			node = node->next;
	}
	node = malloc(sizeof(t_list));
	if (node == 0)
		return (0);
	node->prev = head;
	node->next = head->next;
	node->idx = fd;
	node->line = 0;
	if (node->next)
		head->next->prev = node;
	head->next = node;
	return (node);
}

void	del_node(t_list **node)
{
	free((*node)->line);
	(*node)->line = 0;
	if ((*node)->next)
		(*node)->next->prev = (*node)->prev;
	(*node)->prev->next = (*node)->next;
	free (*node);
	*node = 0;
}

char	*get_next_line(int fd)
{
	static t_list	head;
	t_list			*bac;
	char			*res;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	bac = get_node(&head, fd);
	if (bac == 0)
		return (0);
	bac->line = reader(fd, bac->line);
	if (bac->line == 0)
	{
		del_node(&bac);
		return (0);
	}
	res = get_line(bac->line);
	if (res == 0)
	{
		del_node(&bac);
		return (0);
	}
	bac->line = new_line(bac->line);
	if (bac->line == 0)
		del_node(&bac);
	return (res);
}
