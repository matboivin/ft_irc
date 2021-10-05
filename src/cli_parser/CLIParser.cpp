/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLIParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:34:20 by root              #+#    #+#             */
/*   Updated: 2021/10/05 12:53:07 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CLIParser.hpp"

namespace ft_irc
{
	// default constructor
	CLIParser::CLIParser()
	: argv(), argc(0), bind_address(""), port(""), password("")
	{
		this->argv = nullptr;
	}
	// copy constructor
	CLIParser::CLIParser(const CLIParser& other)
	: argv(), argc(other.argc),
	  bind_address(other.bind_address), port(other.port), password(other.password)
	{
		this->argv = nullptr;
	}

	// arg constructor
	CLIParser::CLIParser(int argc, char** argv)
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

	// assignment operator
	CLIParser&	CLIParser::operator=(const CLIParser& other)
	{
		if (this != &other)
		{
			this->argc = other.argc;
			this->bind_address = other.getBindAddress();
			this->port = other.getPort();
			this->password = other.getPassword();
		}
		return (*this);
	}

	// destructor
	CLIParser::~CLIParser()
	{
	}

	// getters

	std::string	CLIParser::getBindAddress() const
	{
		return (this->bind_address);
	}

	std::string	CLIParser::getPort() const
	{
		return (this->port);
	}

	std::string	CLIParser::getPassword() const
	{
		return (this->password);
	}

	// parse the command line arguments
	void	CLIParser::parse(const char* bind_address, const char* port,
							 const char* password)
	{
		this->bind_address = bind_address;
		this->port = port;
		this->password = password;
	}
}
