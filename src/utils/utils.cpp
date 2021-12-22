/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 16:03:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/21 21:36:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ft_irc.hpp"

void	displayUsage(const char* program_name)
{
	std::cout << "usage: " << program_name
			  << " [host:port_network:password_network] <port> <password>"
			  << std::endl;
}

int	setNonblocking(int fd)
{
	return(fcntl(fd, F_SETFL, O_NONBLOCK));
}
