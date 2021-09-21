/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 16:21:36 by mboivin          ###   ########.fr       */
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
		str_const_it	_packet_it;
		// add next workers in pipeline?

		// helpers
		bool			_parseSeparator();

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
		str_const_it	getPacketIt() const;
		// IRCParser setters
		void			setPacketIt(str_const_it packet_it);

		// tmp main parsing function
		void			parseMessage(const std::string& packet, IRCClient& sender);
		// dummy tmp function
		void			nextStep(Message& msg);
	};
}

#endif // !IRCParser