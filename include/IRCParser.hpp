/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 16:36:44 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include <string>
# include "ft_irc.hpp"

namespace ft_irc
{
	class IRCClient;
	class Message;

	// Parser for IRC protocol messages
	class IRCParser
	{
	private:
		// private alias
		typedef std::string::const_iterator	str_const_it;

		// attributes
		str_const_it	_start;
		str_const_it	_current;
		str_const_it	_end;
		// add next workers in pipeline?

		// helpers
		bool			eat(char expected);
		bool			parseCommand(Message& msg);
		bool			parseParams(Message& msg);
		bool			parseMiddle(Message& msg);
		bool			parseTrailing(Message& msg);
		bool			parseSeparator();
		bool			nospcrlfcl(str_const_it it);

	public:
		// default constructor
						IRCParser();
		// copy constructor
						IRCParser(const IRCParser& other);
		// assignment operator
		IRCParser		&operator=(const IRCParser& other);
		// destructor
						~IRCParser();

		// IRCParser getters
		str_const_it	getItStart() const;
		str_const_it	getItCurrent() const;
		str_const_it	getItEnd() const;
		// IRCParser setters
		void			setItStart(str_const_it start);
		void			setItCurrent(str_const_it current);
		void			setItEnd(str_const_it end);
		void			setIterators(const std::string& str);

		// tmp main parsing function
		Message			parseMessage(const std::string& packet, IRCClient& sender);
		// dummy tmp function
		bool			commandIsValid(Message& msg);
	};
} // !namespace ft_irc

#endif // !IRCParser