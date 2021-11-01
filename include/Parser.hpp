/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/01 18:54:59 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>

namespace ft_irc
{
	class Client;
	class Message;

	/* Parser for IRC protocol messages */
	class Parser
	{
		private:
			/* Aliases */
			typedef std::string::const_iterator	t_str_it;

		public:
			Parser();
			Parser(const Parser& other);
			Parser&	operator=(const Parser& other);
			~Parser();

			/* Getters and setters */
			t_str_it	getItStart() const;
			t_str_it	getItCurrent() const;
			t_str_it	getItEnd() const;

			void		setItStart(t_str_it start);
			void		setItCurrent(t_str_it current);
			void		setItEnd(t_str_it end);
			void		setIterators(const std::string& str);

			/* Main parsing function */
			bool		parseMessage(Message& msg, const std::string& cmd);

		private:
			t_str_it	_start;
			t_str_it	_current;
			t_str_it	_end;

			/* Helpers */
			bool		_commandIsValid(Message& msg);
			bool		_eat(char expected);
			bool		_nocrlf(t_str_it it);
			bool		_nospcrlfcl(t_str_it it);
			bool		_parseSeparator();
			void		_parseTrailing(Message& msg);
			void		_parseMiddle(Message& msg);
			bool		_parseParams(Message& msg);
			bool		_parseCommand(Message& msg);
			void		_fillForwardResponse(Message& msg, std::string cmd);
	};
}

#endif