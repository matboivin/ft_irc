/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 16:03:21 by mboivin           #+#    #+#             */
/*   Updated: 2022/01/21 19:39:35 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ft_irc.hpp"

void	displayUsage(const char* program_name)
{
	std::cout << "usage: " << program_name << " <port> <password>" << std::endl;
}

int	setNonblocking(int fd)
{
	return(fcntl(fd, F_SETFL, O_NONBLOCK));
}
