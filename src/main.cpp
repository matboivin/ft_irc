/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/25 01:24:43 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "server.cpp"
#include "user.hpp"

int	main( int argc, char** argv )
{
	if (argc < 3 || argc > 4)
		displayUsage(argv[0]);

	Server<User> server = Server<User>();
	return (0);
}

