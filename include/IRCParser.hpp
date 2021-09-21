/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:20:57 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 15:25:43 by mboivin          ###   ########.fr       */
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
			IRCParser();
			// copy constructor
			IRCParser(const IRCParser& other);
			// assignment operator
			IRCParser	&operator=(const IRCParser& other);
			~IRCParser();
			// destructor
			void	parse(const std::string& packet, IRCClient& sender); // tmp
			void	nextStep(Message& msg); // dummy tmp function
	};
}

#endif // !IRCParser