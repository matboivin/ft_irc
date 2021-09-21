/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 13:07:21 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/20 15:34:45 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "cli_parser.hpp"

int	main( int argc, char** argv )
{
	try
	{
		ft_irc::CLIParser CLI_parser(argc, argv);
		try
		{
			ft_irc::IRCServer server(CLI_parser.getBindAddress(), CLI_parser.getPort(), CLI_parser.getPassword());
			server.run();
		}
		catch (std::runtime_error &e)
		{
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		displayUsage(argv[0]);
		return EXIT_FAILURE;
	}
	return (EXIT_SUCCESS);
}
