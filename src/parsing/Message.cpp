/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 14:08:40 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Message.hpp"

namespace ft_irc
{
	// default constructor
	Message::Message() : _command(), _params()
	{
	}

	// copy constructor
	Message::Message(const Message& other) : _command(other._command), _params(other._params)
	{
	}

	// assignment operator
	Message&	Message::operator=(const Message& other)
	{
		if (this != &other)
		{
			_command = other.getCommand();
			_params = other.getParams();
		}
		return (*this);
	}

	// destructor
	Message::~Message()
	{
	}

	// getters

	std::string	Message::getCommand(void) const
	{
		return (this->_command);
	}

	Message::str_vec	Message::getParams(void) const
	{
		return (this->_params);
	}

	// setters

	void	Message::setCommand(const std::string& command)
	{
		this->_command = command;
	}

	void	Message::setParam(const std::string& param)
	{
		this->_params.push_back(param);
	}
}