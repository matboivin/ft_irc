/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:47:35 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/22 15:36:11 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include "Config.hpp"

namespace ft_irc
{
	// default constructor
	Config::Config()
	: _hostname("irc.42.fr"),
	  _bind_address("0.0.0.0"),
	  _port("6667"),
	  _connection_password(""),
	  _oper_blocks()
	{
		addOperBlock("Pusheen", "SUPERPASS");
	}

	// copy constructor
	Config::Config(const Config& other)
	: _hostname(other._hostname),
	  _bind_address(other._bind_address),
	  _port(other._port),
	  _connection_password(other._connection_password),
	  _oper_blocks(other._oper_blocks)
	{
	}

	// constructor
	Config::Config(std::string hostname,
				   std::string bind_address,
				   std::string port,
				   std::string password)
	: _hostname(hostname),
	  _bind_address(bind_address),
	  _port(port),
	  _connection_password(password)
	{
		addOperBlock("Pusheen", "SUPERPASS");
	}

	// assignment operator
	Config&	Config::operator=(const Config& other)
	{
		if (this != &other)
		{
			this->_hostname = other.getHostname();
			this->_bind_address = other.getBindAddress();
			this->_port = other.getPort();
			this->_connection_password = other.getPassword();
			this->_oper_blocks = other._oper_blocks;
		}
		return (*this);
	}

	// destructor
	Config::~Config()
	{
	}

	// getters

	std::string	Config::getHostname() const
	{
		return (this->_hostname);
	}

	std::string	Config::getBindAddress() const
	{
		return (this->_bind_address);
	}

	std::string	Config::getPort() const
	{
		return (this->_port);
	}

	std::string	Config::getPassword() const
	{
		return (this->_connection_password);
	}

	// setters

	void	Config::setHostname(const std::string& hostname)
	{
		this->_hostname = hostname;
	}

	void	Config::setBindAddress(const std::string& bind_address)
	{
		this->_bind_address = bind_address;
	}

	void	Config::setPort(const std::string& port)
	{
		this->_port = port;
	}

	void	Config::setPassword(const std::string& password)
	{
		this->_connection_password = password;
	}

	void	Config::addOperBlock(const std::string& name, const std::string& password)
	{
		this->_oper_blocks[name] = password;
	}

	// Checks whether the combination passed as parameter is valid
	bool	Config::operBlockIsValid(const std::string& name, const std::string& password)
	{
		t_oper_block::iterator	it = this->_oper_blocks.find(name);

		if (it != this->_oper_blocks.end())
			return (it->second == password);
		return (false);
	}
} // !namespace ft_irc
