/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_parser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:34:20 by root              #+#    #+#             */
/*   Updated: 2021/09/20 16:46:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cli_parser.hpp"

namespace ft_irc
{
	CLIParser::CLIParser(int argc, char **argv)
	{
		if (argc == 2 && std::strcmp(argv[1], "--help") == 0)
		{
			displayUsage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		if (argc < 4)
			throw std::runtime_error("Not enough arguments");
		if (argc > 4)
			throw std::runtime_error("Too many arguments");
		parse(argv[1], argv[2], argv[3]);
	}
	
	std::string CLIParser::getBindAddress() const
	{
		return this->bind_address;
	}
	std::string CLIParser::getPort() const
	{
		return this->port;
	}
	std::string CLIParser::getPassword() const
	{
		return this->password;
	}
		
	//default constructor
	CLIParser::CLIParser()
	{
		this->argv = nullptr;
		this->argc = 0;
		this->bind_address = "";
		this->port = "";
		this->password = "";
	}
	//copy constructor
	CLIParser::CLIParser(const CLIParser &other)
	{
		this->argv = nullptr;
		this->argc = other.argc;
		this->bind_address = other.bind_address;
		this->port = other.port;
		this->password = other.password;
	}
	//assignment operator
	CLIParser &CLIParser::operator=(const CLIParser &other)
	{
		this->argc = other.argc;
		this->bind_address = other.bind_address;
		this->port = other.port;
		this->password = other.password;
		return *this;
	}
	//destructor
	CLIParser::~CLIParser()
	{
	}
	//parse the command line arguments
	void CLIParser::parse(char const *bind_address, char const *port, char const *password)
	{
		this->bind_address = bind_address;
		this->port = port;
		this->password = password;
	}
}
