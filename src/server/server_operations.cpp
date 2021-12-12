/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 19:37:26 by mboivin           #+#    #+#             */
/*   Updated: 2021/12/12 13:27:32 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "server_operations.hpp"
#include "numeric_replies.hpp"

namespace ft_irc
{
	/* Response helpers */
	std::string	build_prefix(const std::string& sender)
	{
		return (":" + sender);
	}

	std::string	build_full_client_id(const Client& client)
	{
		if (!client.getHostname().empty())
			return (client.getNick() + "!" + client.getUsername() + "@" + client.getHostname());

		// return IP address if host couldn't resolve
		return (client.getNick() + "!" + client.getUsername() + "@" + client.getIpAddressStr());
	}

	/* Late parsing helpers */

	/* Special characters listed in the RFC grammar */
	bool	is_special(char c)
	{
		return (
			(c == '[') || (c == ']') || (c == '\\') || (c == '`')
			|| (c == '_') || (c == '^') || (c == '{') || (c == '}') || (c == '|')
			);
	}

	/*
	 * Check whether a nickname format is valid
	 * A nickname is composed of characters which can be digits, letters or special characters.
	 * It musn't start with a digit.
	 */
	bool	nick_is_valid(const std::string& newnick)
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
		return (false);
	}

	/*
	 * Check channel name
	 * ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring [ ":" chanstring ]
	 */
	bool	channel_is_valid(const std::string& chan_name)
	{
		return ((chan_name[0] == '#') || (chan_name[0] == '+')
				|| (chan_name[0] == '!') || (chan_name[0] == '&'));
	}

	/* Match nicknames */
	bool	match_nick(const std::string& to_match, const std::string& nick)
	{
		//wildcard matching
		if (to_match.find('*') != std::string::npos)
		{
			std::string::size_type	pos = 0;
			std::string::size_type	last_pos = 0;

			while ((pos = to_match.find('*', last_pos)) != std::string::npos)
			{
				if (pos == 0)
					return (true);
				if (pos == to_match.size() - 1)
					return (true);
				if (nick.find(to_match.substr(0, pos)) != std::string::npos)
					return (true);
				last_pos = pos + 1;
			}
			return (false);
		}
		return (to_match == "0" || to_match == nick);
	}

	/* Helper for names */
	bool	is_string_in_msg_params(const Message& msg, const std::string& str)
	{
		for (t_params::const_iterator it = msg.getParams().begin();
			 it != msg.getParams().end();
			 ++it)
		{
			if (*it == str)
				return true;
		}
		return false;
	}

	/* Check mode prefix ( "+" / "-" ) */
	int	verify_mode_prefix(const char& prefix, bool& add_mode)
	{
		if ((prefix == '+') || (prefix == '-'))
		{
			if (prefix == '+')
				add_mode = true;
			else
				add_mode = false;
			return (0);
		}
		return (1);
	}
} // namespace ft_irc
