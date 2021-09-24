/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 17:53:47 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include "IRCParser.hpp"
#include "client.hpp"
#include "Message.hpp"
#include "numeric_replies.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages

	// default constructor
	IRCParser::IRCParser() : _start(), _current(), _end()
	{
	}

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
	IRCParser::~IRCParser()
	{
	}

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

	void	IRCParser::setIterators(const std::string& str)
	{
		this->setItStart(str.begin());
		this->setItCurrent(str.begin());
		this->setItEnd(str.end());
	}

	// parsing

	// Advance iterator if it points to the expected character and returns true
	bool	IRCParser::eat(char expected)
	{
		if ((_current != _end) && (*_current == expected))
		{
			_current++;
			return (true);
		}
		return (false);
	}

	// Checks whether the string contains the CRLF (carriage return + line feed) separator
	bool	IRCParser::parseSeparator()
	{
		if (eat('\r'))
			return (eat('\n'));
		return (false);
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF, space or a colon
	bool	IRCParser::nospcrlfcl(IRCParser::str_const_it it)
	{
		return (it != _end && *it != ' ' && *it != '\r' && *it != '\n' && *it != ':');
	}

	// Any, possibly *empty*, sequence of octets not including NUL or CR or LF
	// *( ":" / " " / nospcrlfcl )
	bool	IRCParser::parseTrailing(Message& msg)
	{
		while ((_current != _end) && (*_current != '\r'))
		{
			if (!nospcrlfcl(_current) && (*_current != ' ') && (*_current != ':'))
				break ;
			_current++;
		}
		msg.setParam(std::string(_start, _current));
		return (parseSeparator());
	}

	// Any *non-empty* sequence of octets not including SPACE or NUL or CR or LF,
	// the first of which may not be ':'
	bool	IRCParser::parseMiddle(Message& msg)
	{
		while (nospcrlfcl(_current) && (*_current != ' '))
			_current++;
		msg.setParam(std::string(_start, _current));
		if (eat(' '))
		{
			_start = _current;
			return (parseTrailing(msg));
		}
		return (parseSeparator());
	}

	// *14(SPACE middle) [ SPACE ":" trailing ]
	bool	IRCParser::parseParams(Message& msg)
	{
		if (eat(' '))
		{
			_start = _current;
			if ((_current != _end) && (*_current == ':'))
				return (parseTrailing(msg));
			else if (_current != _end)
				return (parseMiddle(msg));
		}
		return (parseSeparator());
	}

	// <letter> { <letter> }
	bool	IRCParser::parseCommand(Message& msg)
	{
		if ((_current == _end) || !isalpha(*_current))
			return (false);
		while ((_current != _end) && isalpha(*_current))
			_current++;
		msg.setCommand(std::string(_start, _current));
		return (true);
	}

	// main parsing function
	// not sure about how to pass the client arg
	// packet looks like: <command> [ params ] <crlf>
	// packet max len: 512 characters including CRLF (if not, packet is split)
	void	IRCParser::parseMessage(const std::string& packet, IRCClient& sender)
	{
		Message	msg;

		this->setIterators(packet);
		msg.setSender(sender);

		if (parseCommand(msg)) // wrong command format is ignored
		{
			if (commandIsValid(msg))
				parseParams(msg);
			msg.displayReply(); // debug
			std::cout << '\n';
		}
	}

	// 3 types of messages -> 3 types of handling them:
	//  - respond directly to sender
	//  - forward message to recipient(s)
	//  - broadcast (e.g., joining a channel)

	// dummy tmp function that would pass msg to appropriate worker
	bool	IRCParser::commandIsValid(Message& msg)
	{
		const std::string	cmds[] = {
				"INVITE", "JOIN", "KICK", "KILL", "LIST", "MODE", "NAMES",
				"NICK", "NOTICE", "OPER", "PASS", "PART", "PING", "PONG",
				"PRIVMSG", "TOPIC", "QUIT", "USER", "WHOIS", "WHO"
			};

		for (std::size_t i = 0; i < 20; i++)
		{
			if (cmds[i] == msg.getCommand())
				return (true);
		}
		err_unknowncommand(msg);
		return (false);
	}
}
