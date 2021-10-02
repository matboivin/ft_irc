/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 19:09:25 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/02 20:07:43 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "client.hpp"
#include "Command.hpp"

namespace ft_irc
{
	// nickname constructor
	NickCommand::NickCommand(Message& msg)
	: _newnick(""), _msg(msg)
	{
		try
		{
			setNewNick(msg.getParam(0));
		}
		catch (std::out_of_range& oor)
		{
			err_nonicknamegiven(this->_msg);
		}
	}

	// copy constructor
	NickCommand::NickCommand(const NickCommand& other)
	: _newnick(other._newnick), _msg(other._msg)
	{
	}

	// assignment operator
	NickCommand&	NickCommand::operator=(const NickCommand& other)
	{
		if (this != &other)
		{
			this->_newnick = other.getNewNick();
			this->_msg = other.getMsg();
		}
		return (*this);
	}

	// destructor
	NickCommand::~NickCommand()
	{
	}

	// getters
	std::string	NickCommand::getNewNick() const
	{
		return (this->_newnick);
	}

	Message	NickCommand::getMsg() const
	{
		return (this->_msg);
	}

	// setters
	void	NickCommand::setNewNick(const std::string& newnick)
	{
		this->_newnick = newnick;
	}

	void	NickCommand::setMsg(const Message& msg)
	{
		this->_msg = msg;
	}

	void	NickCommand::execute()
	{
		if (!this->_newnick.empty())
		{
			// TODO
			// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

			// if not valid
			//    err_nicknameinuse(this->_msg);
			// if nickname already exists
			//    err_nicknameinuse(this->_msg);
			// else
			this->_msg.getSender().setNick(_newnick);

			// debug
			std::cout << "client " << this->_msg.getSender().getIpAddressStr() << " nick is now: "
					  <<  this->_msg.getSender().getNick() << std::endl;
		}
	}
} // namespace ft_irc