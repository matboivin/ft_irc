/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/09 17:13:51 by mboivin          ###   ########.fr       */
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
	/* Default constructor */
	Parser::Parser()
	: _start(), _current(), _end()
	{
	}

	/* Copy constructor */
	Parser::Parser(const Parser& other)
	: _start(other._start), _current(other._current), _end(other._end)
	{
	}

	/* Copy assignment operator */
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

	/* Destructor */
	Parser::~Parser()
	{
	}

	/* Getters ****************************************************************** */

	Parser::t_str_it	Parser::getItStart() const
	{
		return (this->_start);
	}

	Parser::t_str_it	Parser::getItCurrent() const
	{
		return (this->_current);
	}

	Parser::t_str_it	Parser::getItEnd() const
	{
		return (this->_end);
	}

	/* Setters ****************************************************************** */

	void	Parser::setItStart(Parser::t_str_it start)
	{
		this->_start = start;
	}

	void	Parser::setItCurrent(t_str_it current)
	{
		this->_current = current;
	}

	void	Parser::setItEnd(Parser::t_str_it end)
	{
		this->_end = end;
	}

	void	Parser::setIterators(const std::string& str)
	{
		this->setItStart(str.begin());
		this->setItCurrent(str.begin());
		this->setItEnd(str.end());
	}

	/* Helpers ****************************************************************** */

	/* Checks whether command name is valid */
	bool	Parser::_commandIsValid(Message& msg)
	{
		const std::string	cmds = "INVITE JOIN KICK KILL LIST MODE NAMES "
									"NICK NOTICE OPER PASS PART PING PONG "
									"PRIVMSG TOPIC QUIT USER WHOIS WHO TEST";

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

	/* If the character is the one expected, advance in the string and returns true */
	bool	Parser::_eat(char expected)
	{
		if ((this->_current != this->_end) && (*this->_current == expected))
		{
			++this->_current;
			return (true);
		}
		return (false);
	}

	/* Checks whether the character pointed by it is not a NUL, CR, LF */
	bool	Parser::_nocrlf(Parser::t_str_it it)
	{
		return (it != this->_end && *it != '\r' && *it != '\n');
	}

	/* Checks whether the character pointed by it is not a NUL, CR, LF, space or a colon */
	bool	Parser::_nospcrlfcl(Parser::t_str_it it)
	{
		return (_nocrlf(it) && *it != ' ' && *it != ':');
	}

	/* Checks whether the string contains the CRLF (carriage return + line feed) separator */
	bool	Parser::_parseSeparator()
	{
		if (_eat('\r'))
			return (_eat('\n'));
		return (false);
	}

	/*
	 * Parses the trailing part of a message
	 * It starts by a ':' followed by a possibly empty sequence of octets not including
	 * NUL or CR or LF
	 */
	void	Parser::_parseTrailing(Message& msg)
	{
		while (_nocrlf(this->_current))
			++this->_current;

		msg.setParam(std::string(this->_start, this->_current));
	}

	/*
	 * Parses the middle part of a message
	 * It musn't start by a ':'.
	 * It is a non-empty sequence of octets not including SPACE or NUL or CR or LF.
	 */
	void	Parser::_parseMiddle(Message& msg)
	{
		while ((_nospcrlfcl(this->_current)) || (*this->_current == ':'))
		{
			++this->_current;
			// KICK can have two lists and requires a special parsing in command execution
			if ((msg.getCommand() != "KICK") && (*this->_current == ','))
			{
				msg.setParam(std::string(this->_start, this->_current));
				_eat(','); // parse list of parameters, ex: JOIN #foo,#bar
				this->_start = this->_current;
			}
		}
		msg.setParam(std::string(this->_start, this->_current));
	}

	/* Parses the command parameters */
	bool	Parser::_parseParams(Message& msg)
	{
		while (_eat(' '))
		{
			this->_start = this->_current;
			if ((this->_current != this->_end) && (*this->_current == ':'))
				_parseTrailing(msg);
			else if (_current != _end)
				_parseMiddle(msg);
		}
		return (_parseSeparator());
	}

	/* Parses the command name */
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

	/* Fill response if Message will be forwarded to other clients */
	void	Parser::_fillForwardResponse(Message& msg, std::string cmd)
	{
		const std::string	cmds = "INVITE JOIN KILL LIST MODE NOTICE OPER "
									"PART PING PONG PRIVMSG QUIT TEST";

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

	/* Main parsing function */
	bool	Parser::parseMessage(Message& msg, const std::string& cmd)
	{
		setIterators(cmd);

		if (_parseCommand(msg))
		{
			if (_commandIsValid(msg))
			{
				_parseParams(msg);
				_fillForwardResponse(msg, cmd.substr( msg.getCommand().size() ));
			}
			return (true);
		}
		return (false);
	}
} // !namespace ft_irc