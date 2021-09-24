/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:01:20 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/24 17:49:00 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string>
# include "ft_irc.hpp"
# include "numeric_replies.hpp"
# include "client.hpp"
# include "Message.hpp"

namespace ft_irc
{
	class IRCClient;

	// helpers
	static std::string	build_prefix(const std::string& from)
	{
		return (":" + from);
	}

	static std::string	build_full_client_id(const IRCClient& client)
	{
		return (
			client.getNick() + "!" + "usertmp" // need username
			+ "@" + client.getIpAddressStr()
			); // need host (ip only if couldn't resolve)
	}

	// numeric replies
	void	rpl_welcome(Message& msg)
	{
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 001 Welcome to the Internet Relay Network "
			+ build_full_client_id(msg.getSender())
			); 
	}

	// error replies
	void	err_unknowncommand(Message& msg)
	{
		msg.setOutput(
			build_prefix(msg.getServHostname())
			+ " 421 " + msg.getCommand() + " :Unknown command"
			);
	}
}