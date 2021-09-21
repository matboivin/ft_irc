/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 19:21:11 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cctype>
#include <string>
#include "IRCParser.hpp"
#include "client.hpp"
#include "Message.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages

	// default constructor
	IRCParser::IRCParser() : _start(), _current(), _end() {}

	// copy constructor
	IRCParser::IRCParser(const IRCParser& other)
			: _start(other._start), _current(other._current), _end(other._end)
	{
	}

	// assignment operator
	IRCParser&	IRCParser::operator=(const IRCParser& other)
	{
		if (this != &other)
		{
			_start = other.getItStart();
			_current = other.getItCurrent();
			_end = other.getItEnd();
		}
		return (*this);
	}

	// destructor
	IRCParser::~IRCParser() {}

	// getters
	IRCParser::str_const_it	IRCParser::getItStart() const
	{
		return (this->_start);
	}

	IRCParser::str_const_it	IRCParser::getItCurrent() const
	{
		return (this->_current);
	}

	IRCParser::str_const_it	IRCParser::getItEnd() const
	{
		return (this->_end);
	}

	// setters
	void	IRCParser::setItStart(IRCParser::str_const_it start)
	{
		this->_start = start;
	}

	void	IRCParser::setItCurrent(str_const_it current)
	{
		this->_current = current;
	}

	void	IRCParser::setItEnd(IRCParser::str_const_it end)
	{
		this->_end = end;
	}

	// parsing

	// <letter> { <letter> } | <number> <number> <number>
	bool	IRCParser::_parseCommand(Message& msg)
	{
		if (!isalnum(*_current))
			return (false);
		if (isdigit(*_current))
		{
			for (int i = 0; i < 3 && (_current != _end); i++)
			{
				if (!isdigit(*_current))
					return (false);
				_current++;
			}
		}
		else if (isalpha(*_current))
		{
			while (_current != _end)
			{
				if (!isalpha(*_current))
					return (false);
				_current++;
			}
		}
		msg.setCommand(std::string(_start, _current));
		return (_parseParams(msg));
	}

	// <SPACE> [ ':' <trailing> | <middle> <params> ]
	bool	IRCParser::_parseParams(Message& msg)
	{
		(void)msg;
		return (_parseSeparator());
	}

	bool	IRCParser::_parseSeparator()
	{
		std::string	s = &(*this->getItStart());

		return (s == CRLF);
	}

	// main parsing function
	// not sure about how to pass the client arg
	// packet looks like: <command> <params> <crlf>
	void	IRCParser::parseMessage(const std::string& packet, IRCClient& sender)
	{
		bool	format_is_correct;
		Message	msg;

		this->setItStart(packet.begin());
		this->setItCurrent(packet.begin());
		this->setItEnd(packet.end());

		_start++; // tmp for ':'
		_current++;

		format_is_correct = _parseCommand(msg);

		if (format_is_correct)
		{
			// tmp
			msg.setPrefix(sender.getNick() + "!" + "tmp" + "@" + sender.getIpAddressStr());
			nextStep(msg);
		}
	}

	// 3 types of messages -> 3 types of handling them:
	//  - respond directly to sender
	//  - forward message to recipient(s)
	//  - broadcast (e.g., joining a channel)

	// dummy tmp function that would pass msg to appropriate worker
	void	IRCParser::nextStep(Message& msg)
	{
		msg.appendSeparator();
		// pass the message
	}
}