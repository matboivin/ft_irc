/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/22 16:30:00 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "Message.hpp"
#include "IRCParser.hpp"

namespace ft_irc
{
	// default constructor
	Message::Message() : _prefix(":"), _command(), _content(), _params() {}

	// copy constructor
	Message::Message(const Message& other)
			: _prefix(other._prefix),
			  _command(other._command),
			  _content(other._content),
			  _params(other._params)
	{
	}

	// assignment operator
	Message&	Message::operator=(const Message& other)
	{
		if (this != &other)
		{
			_prefix = other.getPrefix();
			_command = other.getCommand();
			_content = other.getContent();
			_params = other.getParams();
		}
		return (*this);
	}

	// destructor
	Message::~Message() {}

	// getters

	std::string	Message::getPrefix(void) const
	{
		return (this->_prefix);
	}

	std::string	Message::getCommand(void) const
	{
		return (this->_command);
	}

	std::string	Message::getContent(void) const
	{
		return (this->_content);
	}

	Message::str_vec	Message::getParams(void) const
	{
		return (this->_params);
	}

	// setters

	// <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
	// if cannot resolve the client hostname, the IP address is used instead
	void	Message::setPrefix(const std::string& prefix)
	{
		if (prefix[0] == ':')
			this->_prefix = prefix;
		else
			this->_prefix.append(prefix);
	}

	// <letter> { <letter> } | <number> <number> <number>
	void	Message::setCommand(const std::string& command)
	{
		this->_command = command;
	}

	// Content of the reply or forwarded message
	void	Message::setContent(const std::string& content)
	{
		this->_content = content;
	}

	// <SPACE> [ ':' <trailing> | <middle> <params> ]
	void	Message::setParam(const std::string& param)
	{
		this->_params.push_back(param);
	}

	// end message with CRLF
	void	Message::appendSeparator()
	{
		this->_content.append("\r\n");
	}

	// debug
	void	Message::displayMessage() const
	{
		std::cout << "prefix:  " << getPrefix() << '\n'
				  << "command: " << getCommand() << '\n'
				  << "params:  ";
		
		for (std::size_t i = 0; i < _params.size(); i++)
			std::cout << "'" << _params[i] << "' ";
		std::cout << std::endl;
	}
}