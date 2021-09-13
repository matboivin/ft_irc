/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 16:03:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/12 18:09:29 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ft_irc.hpp"

void	displayUsage( const char* program_name )
{
	std::cout << "usage: " << program_name
			  << " [host:port_network:password_network] <port> <password>"
			  << std::endl;
}
