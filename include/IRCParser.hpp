/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 15:43:12 by mboivin          ###   ########.fr       */
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
	// private:
		// next workers in pipeline?

	public:

		// default constructor
					IRCParser();
		// copy constructor
					IRCParser(const IRCParser& other);
		// assignment operator
		IRCParser	&operator=(const IRCParser& other);
		// destructor
					~IRCParser();

		// tmp main parsing function
		void		parseMessage(const std::string& packet, IRCClient& sender);
		// dummy tmp function
		void		nextStep(Message& msg);
	};
}

#endif // !IRCParser