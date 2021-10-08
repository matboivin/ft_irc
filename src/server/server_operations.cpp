/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:37:26 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/08 16:22:46 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>
#include "Client.hpp"
#include "Message.hpp"
#include "server_operations.hpp"

namespace ft_irc
{
	// Response helpers
	std::string	build_prefix(const std::string& sender)
	{
		return (":" + sender);
	}

	std::string	build_full_client_id(const Client& client)
	{
		return (
			client.getNick() + "!" + client.getUsername()
			+ "@" + client.getIpAddressStr() // need host (ip only if couldn't resolve)
			);
	}

	std::string	fill_msg_response(Message& msg, const std::string& cmd)
	{
		std::string	response = build_prefix( build_full_client_id( msg.getSender() ) ) + ' ' + cmd;

		std::list<std::string>	tmp = msg.getParams();

		while (!tmp.empty())
		{
			response.append(' ' + tmp.front());
			tmp.pop_front();
		}
		response.append("\r\n");
		return (response);
	}

	// Late parsing helpers

	// Special characters listed in the RFC grammar
	bool	is_special(char c)
	{
		return (
			(c == '[') || (c == ']') || (c == '\\') || (c == '`')
			|| (c == '_') || (c == '^') || (c == '{') || (c == '}') || (c == '|')
			);
	}

	// Check whether a nickname format is valid
	// A nickname is composed of 1 to 9 characters which can be digits, letters
	// or special characters. It musn't start with a digit.
	bool	nick_is_valid(const std::string& newnick)
	{
		if (newnick.size() < 10)
		{
			std::string::const_iterator	it = newnick.begin();

			if (isalpha(*it) || is_special(*it))
			{
				++it;
				while (it != newnick.end())
				{
					if (!isalnum(*it) && !is_special(*it) && (*it != '-'))
						return (false);
					++it;
				}
				return (true);
			}
		}
		return (false);
	}

	// Check channel name
	// ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]
	bool	channel_is_valid(const std::string& chan_name)
	{
		return ((chan_name[0] == '#') || (chan_name[0] == '+')
				|| (chan_name[0] == '!') || (chan_name[0] == '&'));
	}
} // namespace ft_irc