/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:21 by mboivin           #+#    #+#             */
/*   Updated: 2022/10/22 22:00:27 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "ft_irc.hpp"

int	main(int argc, char** argv)
{
#if defined(DEBUG_PARSING)
	testParsing();
	return (0);
#endif
	try
	{
		ft_irc::CLIParser	CLI_parser(argc, argv);

		try
		{
			ft_irc::Server	server(CLI_parser);

			server.run();
		}
		catch (std::runtime_error& e)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << e.what() << std::endl;
			return (EXIT_FAILURE);
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << __FILE__ ":" <<__LINE__ << " : " << e.what() << std::endl;
		displayUsage(argv[0]);
		return (EXIT_FAILURE);
	}
	std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: Exiting." << std::endl;
	return (EXIT_SUCCESS);
}
