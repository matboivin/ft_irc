/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/22 15:17:20 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include <string>
# include "ft_irc.hpp"
# include "client.hpp"
# include "Message.hpp"

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
		bool			_parseCommand(Message& msg);
		bool			_parseParams(Message& msg);
		bool			_parseMiddle(Message& msg);
		bool			_parseTrailing(Message& msg);
		bool			_parseSeparator();
		bool			_nospcrlfcl(str_const_it it);

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

		// tmp main parsing function
		void			parseMessage(const std::string& packet, IRCClient& sender);
		// dummy tmp function
		void			nextStep(Message& msg);
	};
}

#endif // !IRCParser