/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/22 18:29:57 by mboivin          ###   ########.fr       */
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

	void	IRCParser::setIterators(const std::string& str)
	{
		this->setItStart(str.begin());
		this->setItCurrent(str.begin());
		this->setItEnd(str.end());
	}

	// parsing

	// Advance iterator if it points to the expected character and returns true
	bool	IRCParser::_eat(char expected)
	{
		if ((_current != _end) && (*_current == expected))
		{
			_current++;
			return (true);
		}
		return (false);
	}

	// Checks whether the string contains the CRLF (carriage return + line feed) separator
	bool	IRCParser::_parseSeparator()
	{
		if (_eat('\r'))
			return (_eat('\n'));
		return (false);
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF, space or a colon
	bool	IRCParser::_nospcrlfcl(IRCParser::str_const_it it)
	{
		return (it != _end && *it != ' ' && *it != '\r' && *it != '\n' && *it != ':');
	}

	// Any, possibly *empty*, sequence of octets not including NUL or CR or LF
	// *( ":" / " " / nospcrlfcl )
	bool	IRCParser::_parseTrailing(Message& msg)
	{
		while ((_current != _end) && (*_current != '\r'))
		{
			if (!_nospcrlfcl(_current) && (*_current != ' ') && (*_current != ':'))
				break ;
			_current++;
		}
		msg.setParam(std::string(_start, _current));
		return (_parseSeparator());
	}

	// Any *non-empty* sequence of octets not including SPACE or NUL or CR or LF,
	// the first of which may not be ':'
	bool	IRCParser::_parseMiddle(Message& msg)
	{
		while (_nospcrlfcl(_current) && (*_current != ' '))
			_current++;
		msg.setParam(std::string(_start, _current));
		if (_eat(' '))
		{
			_start = _current;
			return (_parseTrailing(msg));
		}
		return (_parseSeparator());
	}

	// *14(SPACE middle) [ SPACE ":" trailing ]
	bool	IRCParser::_parseParams(Message& msg)
	{
		if (_eat(' '))
		{
			_start = _current;
			if ((_current != _end) && (*_current == ':'))
				return (_parseTrailing(msg));
			else if (_current != _end)
				return (_parseMiddle(msg));
		}
		return (_parseSeparator());
	}

	// <letter> { <letter> }
	bool	IRCParser::_parseCommand(Message& msg)
	{
		if ((_current == _end) || !isalpha(*_current))
			return (false);
		while ((_current != _end) && isalpha(*_current))
			_current++;
		msg.setCommand(std::string(_start, _current));
		return (_parseParams(msg));
	}

	// main parsing function
	// not sure about how to pass the client arg
	// packet looks like: <command> [ params ] <crlf>
	void	IRCParser::parseMessage(const std::string& packet, IRCClient& sender)
	{
		bool	format_is_correct;
		Message	msg;

		this->setIterators(packet);

		format_is_correct = _parseCommand(msg);

		if (format_is_correct)
		{
			// tmp
			// example forward
			msg.setPrefix(sender.getNick() + "!" + "tmp" + "@" + sender.getIpAddressStr());
			msg.setContent(packet);
			// example numeric reply
			// msg.setPrefix("ft_irc"); // server hostname
			// msg.setCommand("001");
			// msg.setContent("Welcome to the Internet Relay Network "
			// 				+ sender.getNick() + "!" + "tmp" + "@"
			// 				+ sender.getIpAddressStr());
		}

		msg.displayMessage(); // debug
		std::cout << "\n\n";

		nextStep(msg);
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