/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/19 16:55:09 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <map>
# include <vector>

namespace ft_irc
{
	class Client;
	class Message;

	/*
	 * Parser for IRC protocol messages
	 * Attribute of the Server
	 */
	class Parser
	{
	private:
		/* Internal aliases */
		typedef std::string::const_iterator			t_str_it;
		typedef std::map<std::string, std::size_t>	t_cmd_infos;

	public:
		/* Alias */
		typedef std::vector<std::string>			t_params;

		/* Default constructor */
					Parser();

		/* Copy constructor */
					Parser(const Parser& other);

		/* Copy assignment operator */
		Parser&		operator=(const Parser& other);

		/* Destructor */
					~Parser();

		/* Getters */
		t_str_it	getItStart() const;
		t_str_it	getItCurrent() const;
		t_str_it	getItEnd() const;
		t_cmd_infos	getCmdInfos() const;

		/* Setters */
		void		setItStart(t_str_it start);
		void		setItCurrent(t_str_it current);
		void		setItEnd(t_str_it end);
		void		setIterators(const std::string& str);
		void		setCmdInfos(const t_cmd_infos& cmd_infos);

		/* Helpers */
		t_params	splitListOfParams(const std::string& params);

		/* Main parsing function */
		bool		parseMessage(Message& msg, const std::string& cmd);

	private:
		/* Attributes */
		t_str_it	_start;
		t_str_it	_current;
		t_str_it	_end;
		t_cmd_infos	_cmd_infos;

		/* Helpers */
		void		_initCmdInfos();
		void		_handleListOfParams(Message& msg);
		bool		_commandIsValid(Message& msg);
		bool		_eat(char expected);
		bool		_nocrlf(t_str_it it);
		bool		_nospcrlfcl(t_str_it it);
		bool		_parseSeparator();
		bool		_parseTrailing(Message& msg);
		void		_parseMiddle(Message& msg);
		bool		_parseParams(Message& msg);
		bool		_parseCommand(Message& msg);
		void		_fillForwardResponse(Message& msg);
	};
}

#endif // !PARSER_HPP
