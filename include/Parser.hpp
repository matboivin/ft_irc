/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/01 17:15:17 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>

namespace ft_irc
{
	class Client;
	class Message;

	// Parser for IRC protocol messages
	class Parser
	{
	private:
		// internal alias
		typedef std::string::const_iterator	t_str_it;

		// attributes
		t_str_it	_start;
		t_str_it	_current;
		t_str_it	_end;

		// helpers
		bool	_commandIsValid(Message& msg);
		bool	_eat(char expected);
		bool	_nocrlf(t_str_it it);
		bool	_nospcrlfcl(t_str_it it);
		bool	_parseSeparator();
		void	_parseTrailing(Message& msg);
		void	_parseMiddle(Message& msg);
		bool	_parseParams(Message& msg);
		bool	_parseCommand(Message& msg);
		void	_fillForwardResponse(Message& msg, std::string cmd);

	public:
		// default constructor
				Parser();
		// copy constructor
				Parser(const Parser& other);
		// assignment operator
		Parser&	operator=(const Parser& other);
		// destructor
				~Parser();

		// getters
		t_str_it	getItStart() const;
		t_str_it	getItCurrent() const;
		t_str_it	getItEnd() const;

		// setters
		void	setItStart(t_str_it start);
		void	setItCurrent(t_str_it current);
		void	setItEnd(t_str_it end);
		void	setIterators(const std::string& str);

		// main parsing function
		bool	parseMessage(Message& msg, const std::string& cmd);
	};
} // !namespace ft_irc

#endif // !Parser