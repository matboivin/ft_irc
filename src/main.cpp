/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/12 13:27:47 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "ft_irc.hpp"

int	main(int argc, char** argv)
{
#if defined(DEBUG_PARSING)
	test_parsing();
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
