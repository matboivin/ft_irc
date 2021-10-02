/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Cr_eated: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 16:54:50 by mboivin          ###   ########.fr       */
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
	IRCParser::IRCParser()
	: _start(), _current(), _end()
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

	// Checks whether command name is valid
	bool	IRCParser::_commandIsValid(Message& msg)
	{
		const std::string	cmds[] = {
				"INVITE", "JOIN", "KICK", "KILL", "LIST", "MODE", "NAMES",
				"NICK", "NOTICE", "OPER", "PASS", "PART", "PING", "PONG",
				"PRIVMSG", "TOPIC", "QUIT", "USER", "WHOIS", "WHO"
			};
		std::string	cmd_name = msg.getCommand();

		std::transform(cmd_name.begin(), cmd_name.end(), cmd_name.begin(), ::toupper);

		for (std::size_t i = 0; i < 20; i++)
		{
			if (cmds[i] == cmd_name)
			{
				msg.setCommand(cmd_name); // uppercase command name if valid
				return (true);
			}
		}
		err_unknowncommand(msg); // else keep original format
		return (false);
	}

	// parsing helpers

	// If the character is the one expected, advance in the string and returns true
	bool	IRCParser::_eat(char expected)
	{
		if ((this->_current != this->_end) && (*this->_current == expected))
		{
			this->_current++;
			return (true);
		}
		return (false);
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF
	bool	IRCParser::_nocrlf(IRCParser::str_const_it it)
	{
		return (it != this->_end && *it != '\r' && *it != '\n');
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF, space or a colon
	bool	IRCParser::_nospcrlfcl(IRCParser::str_const_it it)
	{
		return (_nocrlf(it) && *it != ' ' && *it != ':');
	}

	// Checks whether the string contains the CRLF (carriage return + line feed) separator
	bool	IRCParser::_parseSeparator()
	{
		if (_eat('\r'))
			return (_eat('\n'));
		return (false);
	}

	// Parses the trailing part of a message
	// It starts by a ':' followed by a possibly empty sequence of octets not including
	// NUL or CR or LF
	bool	IRCParser::_parseTrailing(Message& msg)
	{
		while ((this->_current != this->_end) && (*this->_current != '\r'))
		{
			if (!_nocrlf(this->_current))
				break ;
			this->_current++;
		}
		msg.setParam(std::string(this->_start, this->_current));
		return (_parseSeparator());
	}

	// Parses the middle part of a message
	// It musn't start by a ':'.
	// It is a non-empty sequence of octets not including SPACE or NUL or CR or LF.
	bool	IRCParser::_parseMiddle(Message& msg)
	{
		while (_nospcrlfcl(this->_current) || (*this->_current == ':'))
			this->_current++;
		msg.setParam(std::string(this->_start, this->_current));
		if (_eat(' '))
		{
			this->_start = this->_current;
			return (_parseTrailing(msg));
		}
		return (_parseSeparator());
	}

	// Parses the command parameters
	bool	IRCParser::_parseParams(Message& msg)
	{
		if (_eat(' '))
		{
			this->_start = this->_current;
			if ((this->_current != this->_end) && (*this->_current == ':'))
				return (_parseTrailing(msg));
			else if (_current != _end)
				return (_parseMiddle(msg));
		}
		return (_parseSeparator());
	}

	// Parses the command name
	bool	IRCParser::_parseCommand(Message& msg)
	{
		if ((this->_current == this->_end) || !isalpha(*this->_current))
			return (false);
		while ((this->_current != this->_end) && isalpha(*this->_current))
			this->_current++;

		std::string	cmd(this->_start, this->_current);
		msg.setCommand(cmd);
		return (true);
	}

	// Main parsing function
	Message	IRCParser::parseMessage(const std::string& packet, IRCClient& sender)
	{
		Message	msg;

		setIterators(packet);
		msg.setSender(sender);

		if (_parseCommand(msg)) // wrong command name format is silently ignored
		{
			if (_commandIsValid(msg))
				_parseParams(msg);
		}
		return (msg);
	}
} // !namespace ft_irc