/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CLIParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:34:20 by root              #+#    #+#             */
/*   Updated: 2021/10/05 14:08:34 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CLIParser.hpp"

namespace ft_irc
{
	// default constructor
	CLIParser::CLIParser()
	: _argv(), _argc(0), _bind_address(""), _port(""), _password("")
	{
		this->_argv = nullptr;
	}
	// copy constructor
	CLIParser::CLIParser(const CLIParser& other)
	: _argv(), _argc(other._argc),
	  _bind_address(other._bind_address), _port(other._port),
	  _password(other._password)
	{
		this->_argv = nullptr;
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
			this->_argc = other._argc;
			this->_bind_address = other.getBindAddress();
			this->_port = other.getPort();
			this->_password = other.getPassword();
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
		return (this->_bind_address);
	}

	std::string	CLIParser::getPort() const
	{
		return (this->_port);
	}

	std::string	CLIParser::getPassword() const
	{
		return (this->_password);
	}

	// parse the command line arguments
	void	CLIParser::parse(const char* bind_address, const char* port,
							 const char* password)
	{
		this->_bind_address = bind_address;
		this->_port = port;
		this->_password = password;
	}
}
