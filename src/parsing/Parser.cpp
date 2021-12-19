/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/19 16:55:27 by mboivin          ###   ########.fr       */
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
#include "ft_irc.hpp"

namespace ft_irc
{
	/* Default constructor */
	Parser::Parser()
	: _start(), _current(), _end(), _cmd_infos()
	{
		_initCmdInfos();
	}

	/* Copy constructor */
	Parser::Parser(const Parser& other)
	: _start(other._start), _current(other._current), _end(other._end), _cmd_infos(other._cmd_infos)
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
			_cmd_infos = other.getCmdInfos();
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

	Parser::t_cmd_infos	Parser::getCmdInfos() const
	{
		return (this->_cmd_infos);
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

	void	Parser::setCmdInfos(const t_cmd_infos& cmd_lenghts)
	{
		this->_cmd_infos = cmd_lenghts;
	}

	/* Helpers ****************************************************************** */

	/* Split list of params such as: #foo,#bar -> '#foo' '#bar' */
	Parser::t_params	Parser::splitListOfParams(const std::string& params)
	{
		t_params	result;

		if (params.find(",") == std::string::npos)
		{
			result.push_back(params);
			return (result);
		}

		std::string::const_iterator	current = params.begin();
		std::string::const_iterator	start = current;

		while (current != params.end())
		{
			if (*current == ',')
			{
				result.push_back(std::string(start, current));
				++current;
				start = current;
			}
			++current;
		}
		result.push_back(std::string(start, current));
		return (result);
	}

	/* Init max number of params for each command */
	void	Parser::_initCmdInfos()
	{
		this->_cmd_infos["DIE"]		= 0;
		this->_cmd_infos["INVITE"]	= 2;
		this->_cmd_infos["JOIN"]	= 1;
		this->_cmd_infos["KICK"]	= 3;
		this->_cmd_infos["KILL"]	= 2;
		this->_cmd_infos["LIST"]	= 1;
		this->_cmd_infos["MODE"]	= 3;
		this->_cmd_infos["MOTD"]	= 1;
		this->_cmd_infos["NICK"]	= 1;
		this->_cmd_infos["NAMES"]	= 1;
		this->_cmd_infos["NOTICE"]	= 2;
		this->_cmd_infos["OPER"]	= 2;
		this->_cmd_infos["PART"]	= 2;
		this->_cmd_infos["PASS"]	= 1;
		this->_cmd_infos["PING"]	= 2;
		this->_cmd_infos["PONG"]	= 2;
		this->_cmd_infos["PRIVMSG"]	= 2;
		this->_cmd_infos["QUIT"]	= 1;
		this->_cmd_infos["TOPIC"]	= 2;
		this->_cmd_infos["USER"]	= 4;
		this->_cmd_infos["WHO"]		= 2;
		this->_cmd_infos["WHOIS"]	= 1;
	}

	/* Checks whether command name is valid */
	void	Parser::_handleListOfParams(Message& msg)
	{
		const std::string	cmds = "LIST JOIN NAMES WHOIS";

		if (cmds.find(msg.getCommand()) == std::string::npos)
			return ;
		if (msg.getParams().empty())
			return ;

		std::size_t	len = msg.getParams().size();
		t_params	params;
		t_params	tmp;

		for (std::size_t i = 0; i < len; ++i)
		{
			tmp = splitListOfParams(msg.getParams().at(i));
			params.insert(params.end(), tmp.begin(), tmp.end());
		}
		msg.setParams(params);
	}

	/* Checks whether command name is valid */
	bool	Parser::_commandIsValid(Message& msg)
	{
		std::string	cmd_name = msg.getCommand();

		std::transform(cmd_name.begin(), cmd_name.end(), cmd_name.begin(), ::toupper);

		if ((cmd_name.at(0) == '/') && (this->_cmd_infos.find(cmd_name.substr(1)) != this->_cmd_infos.end()))
		{
			err_unknowncommand(msg, "; the correct syntax is " + cmd_name.substr(1));
			return (false);
		}
		else if (this->_cmd_infos.find(cmd_name) != this->_cmd_infos.end())
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
	bool	Parser::_parseTrailing(Message& msg)
	{
		while (_nocrlf(this->_current))
			++this->_current;
		std::string	param = std::string(this->_start, this->_current);
		if (param.size() > 1) // single ':'
			msg.setParam(param);
		return (_parseSeparator());
	}

	/*
	 * Parses the middle part of a message
	 * It musn't start by a ':'.
	 * It is a non-empty sequence of octets not including SPACE or NUL or CR or LF.
	 */
	void	Parser::_parseMiddle(Message& msg)
	{
		while ((_nospcrlfcl(this->_current)) || (*this->_current == ':'))
			++this->_current;
		msg.setParam(std::string(this->_start, this->_current));
	}

	/* Parses the command parameters */
	bool	Parser::_parseParams(Message& msg)
	{
		while (_eat(' '))
		{
			this->_start = this->_current;
			if ((this->_current != this->_end) && (*this->_current == ':'))
				return (_parseTrailing(msg));
			else if (_current != _end)
				_parseMiddle(msg);
		}
		return (_parseSeparator());
	}

	/* Parses the command name */
	bool	Parser::_parseCommand(Message& msg)
	{
		if ((this->_current != this->_end) && (*this->_current == '/'))
			++this->_current;
		if ((this->_current == this->_end) || !isalpha(*this->_current))
			return (false);
		while ((this->_current != this->_end) && isalpha(*this->_current))
			++this->_current;

		std::string	cmd(this->_start, this->_current);
		msg.setCommand(cmd);
		return (true);
	}

	/* Fill response to be forwarded to other clients */
	void	Parser::_fillForwardResponse(Message& msg)
	{
		const std::string	cmds = "KICK KILL MODE WHOIS"; // no need to fill

		if (cmds.find(msg.getCommand()) != std::string::npos)
			return ;

		t_cmd_infos::const_iterator	it = this->_cmd_infos.find(msg.getCommand());

		if (it != this->_cmd_infos.end())
		{
			std::size_t	i = 0;
			std::size_t	len = msg.getParams().size();

			msg.setResponse(build_prefix(build_full_client_id(msg.getSender())));
			msg.appendResponse(" ");
			msg.appendResponse(msg.getCommand());

			while (i < it->second && i < len)
			{
				msg.appendResponse(" ");
				msg.appendResponse(msg.getParams().at(i));
				++i;
			}
			msg.appendSeparator();
		}
	}

	/* Main parsing function */
	bool	Parser::parseMessage(Message& msg, const std::string& cmd)
	{
		Logger	logger(DEBUG);

		setIterators(cmd);

		if (_parseCommand(msg))
		{
			if (_commandIsValid(msg))
			{
				_parseParams(msg);
				_fillForwardResponse(msg);
				_handleListOfParams(msg); // split list of params (param1,param2)
			}
			return (true);
		}
		logger.log(LOG_LEVEL_INFO, "Invalid command is silently ignored (" + cmd + ")");
		return (false);
	}
} // !namespace ft_irc
