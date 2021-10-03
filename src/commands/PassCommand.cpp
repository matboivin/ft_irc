/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 15:18:05 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 15:35:20 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include "Message.hpp"
#include "client.hpp"
#include "Command.hpp"

namespace ft_irc
{
	// message constructor
	PassCommand::PassCommand(Message& msg)
	: _msg(msg)
	{
	}

	// copy constructor
	PassCommand::PassCommand(const PassCommand& other)
	: _msg(other._msg)
	{
	}

	// assignment operator
	PassCommand&	PassCommand::operator=(const PassCommand& other)
	{
		if (this != &other)
			this->_msg = other.getMsg();

		return (*this);
	}

	// destructor
	PassCommand::~PassCommand()
	{
	}

	// getters
	Message	PassCommand::getMsg() const
	{
		return (this->_msg);
	}

	// setters
	void	PassCommand::setMsg(const Message& msg)
	{
		this->_msg = msg;
	}

	// Set a connection password
	void	PassCommand::execute()
	{
		if (this->_msg.getParams().empty())
			err_needmoreparams(this->_msg);
		else if (this->_msg.getSender().isRegistered())
			err_alreadyregistered(this->_msg);
		else
			this->_msg.getSender().setPassword(this->_msg.getParam(0));
	}
} // namespace ft_irc