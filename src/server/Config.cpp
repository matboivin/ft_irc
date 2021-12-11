/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:47:35 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/11 16:41:13 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include "Config.hpp"

namespace ft_irc
{
	/* Default constructor */
	Config::Config()
	: _bind_address("0.0.0.0"),
	  _port("6667"),
	  _connection_password(""),
	  _hostname("irc.42.fr"),
	  _oper_blocks()
	{
		addOperBlock("user42", "pass42");
	}

	/* Copy constructor */
	Config::Config(const Config& other)
	: _bind_address(other._bind_address),
	  _port(other._port),
	  _connection_password(other._connection_password),
	  _hostname(other._hostname),
	  _oper_blocks(other._oper_blocks)
	{
	}

	/* Settings constructor */
	Config::Config(const std::string& bind_address,
				   const std::string& port,
				   const std::string& password,
				   const std::string& hostname)
	: _bind_address(bind_address),
	  _port(port),
	  _connection_password(password),
	  _hostname(hostname),
	  _oper_blocks()
	{
		addOperBlock("user42", "pass42");
	}

	/* Copy assignment operator */
	Config&	Config::operator=(const Config& other)
	{
		if (this != &other)
		{
			this->_bind_address = other._bind_address;
			this->_port = other._port;
			this->_connection_password = other._connection_password;
			this->_hostname = other._hostname;
			this->_oper_blocks = other._oper_blocks;
		}
		return (*this);
	}

	/* Destructor */
	Config::~Config()
	{
	}

	/* Getters ****************************************************************** */

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

	std::string	Config::getHostname() const
	{
		return (this->_hostname);
	}

	const Config::t_oper_block&	Config::getOperBlocks() const
	{
		return (this->_oper_blocks);
	}

	/* Setters ****************************************************************** */

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

	void	Config::setHostname(const std::string& hostname)
	{
		this->_hostname = hostname;
	}

	/* Oper operations ********************************************************** */

	/* Add an oper block (name,password) */
	void	Config::addOperBlock(const std::string& name, const std::string& password)
	{
		this->_oper_blocks[name] = password;
	}

	void	Config::clearOperBlocks()
	{
		this->_oper_blocks.clear();
	}

	/* Checks whether the combination passed as parameter is valid */
	bool	Config::operBlockIsValid(const std::string& name, const std::string& password)
	{
		t_oper_block::iterator	it = this->_oper_blocks.find(name);

		if (it != this->_oper_blocks.end())
			return (it->second == password);
		return (false);
	}
} // !namespace ft_irc
