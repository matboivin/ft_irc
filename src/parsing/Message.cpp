/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/22 18:44:11 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "Message.hpp"
#include "IRCParser.hpp"

namespace ft_irc
{
	// default constructor
	Message::Message()
			: _prefix(), _command(), _content(), _params(), _type(undefined)
	{
	}

	// copy constructor
	Message::Message(const Message& other)
			: _prefix(other._prefix),
			  _command(other._command),
			  _content(other._content),
			  _params(other._params),
			  _type(other._type)
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
			_type = other.getType();
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

	reply_type	Message::getType() const
	{
		return (this->_type);
	}

	// setters

	void	Message::setPrefix(const std::string& prefix)
	{
		if (prefix.empty())
			return ;
		if (prefix[0] == ':')
			this->_prefix = prefix;
		else
			this->_prefix = ":" + prefix;
	}

	void	Message::setCommand(const std::string& command)
	{
		this->_command = command;
	}

	void	Message::setContent(const std::string& content)
	{
		this->_content = content;
	}

	void	Message::setParam(const std::string& param)
	{
		if (!param.empty())
			this->_params.push_back(param);
	}

	void	Message::setType(reply_type type)
	{
		this->_type = type;
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