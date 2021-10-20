/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/09 17:57:59 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "numeric_replies.hpp"
#include "server_operations.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages

	// default constructor
	Parser::Parser()
	: _start(), _current(), _end()
	{
	}

	// copy constructor
	Parser::Parser(const Parser& other)
	: _start(other._start), _current(other._current), _end(other._end)
	{
	}

	// assignment operator
	Parser&	Parser::operator=(const Parser& other)
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
	Parser::~Parser()
	{
	}

	// getters
	Parser::str_const_it	Parser::getItStart() const
	{
		return (this->_start);
	}

	Parser::str_const_it	Parser::getItCurrent() const
	{
		return (this->_current);
	}

	Parser::str_const_it	Parser::getItEnd() const
	{
		return (this->_end);
	}

	// setters
	void	Parser::setItStart(Parser::str_const_it start)
	{
		this->_start = start;
	}

	void	Parser::setItCurrent(str_const_it current)
	{
		this->_current = current;
	}

	void	Parser::setItEnd(Parser::str_const_it end)
	{
		this->_end = end;
	}

	void	Parser::setIterators(const std::string& str)
	{
		this->setItStart(str.begin());
		this->setItCurrent(str.begin());
		this->setItEnd(str.end());
	}

	// Checks whether command name is valid
	bool	Parser::_commandIsValid(Message& msg)
	{
		const std::string	cmds = "INVITE JOIN KICK KILL LIST MODE NAMES "
									"NICK NOTICE OPER PASS PART PING PONG "
									"PRIVMSG TOPIC QUIT USER WHOIS WHO";

		std::string	cmd_name = msg.getCommand();
		std::transform(cmd_name.begin(), cmd_name.end(), cmd_name.begin(), ::toupper);

		if (cmds.find(cmd_name) != std::string::npos)
		{
			msg.setCommand(cmd_name); // uppercase command name if valid
			return (true);
		}
		err_unknowncommand(msg); // else keep original format
		return (false);
	}

	// parsing helpers

	// If the character is the one expected, advance in the string and returns true
	bool	Parser::_eat(char expected)
	{
		if ((this->_current != this->_end) && (*this->_current == expected))
		{
			++this->_current;
			return (true);
		}
		return (false);
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF
	bool	Parser::_nocrlf(Parser::str_const_it it)
	{
		return (it != this->_end && *it != '\r' && *it != '\n');
	}

	// Checks whether the character pointed by it is not a NUL, CR, LF, space or a colon
	bool	Parser::_nospcrlfcl(Parser::str_const_it it)
	{
		return (_nocrlf(it) && *it != ' ' && *it != ':');
	}

	// Checks whether the string contains the CRLF (carriage return + line feed) separator
	bool	Parser::_parseSeparator()
	{
		if (_eat('\r'))
			return (_eat('\n'));
		return (false);
	}

	// Parses the trailing part of a message
	// It starts by a ':' followed by a possibly empty sequence of octets not including
	// NUL or CR or LF
	void	Parser::_parseTrailing(Message& msg)
	{
		while (_nocrlf(this->_current))
			++this->_current;

		msg.setParam(std::string(this->_start, this->_current));;
	}

	// Parses the middle part of a message
	// It musn't start by a ':'.
	// It is a non-empty sequence of octets not including SPACE or NUL or CR or LF.
	void	Parser::_parseMiddle(Message& msg)
	{
		while ((_nospcrlfcl(this->_current) || (*this->_current == ':')) && (*this->_current != ','))
			++this->_current;
		msg.setParam(std::string(this->_start, this->_current));
	}

	// Parses the command parameters
	bool	Parser::_parseParams(Message& msg)
	{
		// the comma means it's a list of parameters (e.g., <channel>,<channel>)
		while (_eat(' ') || _eat(','))
		{
			this->_start = this->_current;
			if ((this->_current != this->_end) && (*this->_current == ':'))
				_parseTrailing(msg);
			else if (_current != _end)
				_parseMiddle(msg);
		}
		return (_parseSeparator());
	}

	// Parses the command name
	bool	Parser::_parseCommand(Message& msg)
	{
		if ((this->_current == this->_end) || !isalpha(*this->_current))
			return (false);
		while ((this->_current != this->_end) && isalpha(*this->_current))
			++this->_current;

		std::string	cmd(this->_start, this->_current);
		msg.setCommand(cmd);
		return (true);
	}

	// Fill response if Message will be forwarded to other clients
	void	Parser::_fillForwardResponse(Message& msg, std::string cmd)
	{
		const std::string	cmds = "INVITE JOIN KICK KILL LIST MODE NOTICE OPER "
									"PART PING PONG PRIVMSG QUIT";

		if (cmds.find(msg.getCommand()) != std::string::npos)
		{
			std::string	response = build_prefix( build_full_client_id( msg.getSender() ) ) + ' ' + msg.getCommand();

			if (cmd.size() > 0) // trim spaces
			{
				std::string::iterator	it = cmd.begin();
				std::string::iterator	ite = cmd.end();

				while (it != cmd.end() && *it == ' ')
					++it;
				while (ite != it && *ite == ' ')
					--ite;

				std::string	params(it, ite);

				if (!params.empty())
					response.append(' ' + params);
			}
			msg.setResponse(response);
			msg.appendSeparator();
		}
	}

	// Main parsing function
	Message	Parser::parseMessage(Client& sender, const std::string& cmd)
	{
		Message	msg(sender);

		setIterators(cmd);

		if (_parseCommand(msg)) // wrong command name format is silently ignored
		{
			if (_commandIsValid(msg))
			{
				_parseParams(msg);
				_fillForwardResponse(msg, cmd.substr( msg.getCommand().size() ));
			}
		}
		return (msg);
	}
} // !namespace ft_irc