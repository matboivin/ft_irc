/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/02 19:09:25 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 15:18:54 by mboivin          ###   ########.fr       */
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

	// Special characters listed in the RFC grammar
	static bool	is_special(char c)
	{
		return (
			(c == '[') || (c == ']') || (c == '\\') || (c == '`')
			|| (c == '_') || (c == '^') || (c == '{') || (c == '}') || (c == '|')
			);
	}

	// Check whether a nickname format is valid
	// A nickname is composed of 1 to 9 characters which can be digits, letters or special characters.
	// It musn't start by a digit
	bool	NickCommand::_nick_is_valid()
	{
		if (this->_newnick.size() > 9)
			return (false);

		std::string::const_iterator	it = this->_newnick.begin();

		if (isalpha(*it) || is_special(*it))
		{
			++it;
			while (it != this->_newnick.end())
			{
				if (!isalnum(*it) && !is_special(*it) && (*it != '-'))
					return (false);
				++it;
			}
			return (true);
		}
		return (false);
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
			if (!_nick_is_valid())
			{
				err_erroneusnickname(this->_msg, this->_newnick);
			}
			// else if nickname already exists
			//    err_nicknameinuse(this->_msg);
			else
			{
				this->_msg.getSender().setNick(_newnick);
				// debug
				std::cout << "client " << this->_msg.getSender().getIpAddressStr()
						  << " nick is now: " <<  this->_msg.getSender().getNick()
						  << std::endl;
			}
		}
	}
} // namespace ft_irc